﻿/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Core/Manager/FrameBufferManager.h"
#include "Core/Manager/TextureManager.h"
#include "Core/Engine.h"

#include "Core/Renderer/Texture.h"
#include "Core/Renderer/FrameBuffer.h"
#include "Core/Renderer/BaseGraphics.h"

namespace tezcat::Tiny
{
	FrameBuffer* FrameBufferManager::sDefaultBuffer = nullptr;
	uint32_t FrameBufferManager::sID = 0;
	std::queue<uint32_t> FrameBufferManager::sFreeIDs;
	std::unordered_map<std::string_view, FrameBuffer*> FrameBufferManager::sUMap;
	FrameBuffer* FrameBufferManager::sMainBuffer = nullptr;

	void FrameBufferManager::init()
	{
		sDefaultBuffer = FrameBuffer::create("FB_Tiny_Default");
		sDefaultBuffer->saveObject();
	}

	std::tuple<FlagCreate, FrameBuffer*> FrameBufferManager::create(std::string name)
	{
		auto result = sUMap.find(name);
		if (result != sUMap.end())
		{
			return { FlagCreate::Existed, result->second };
		}

		auto frame_buffer = FrameBuffer::create(name);
		frame_buffer->saveObject();
		sUMap.emplace(frame_buffer->getEngineName().toView(), frame_buffer);
		return { FlagCreate::Succeeded, frame_buffer };
	}

	FrameBuffer* FrameBufferManager::getMainFrameBufferBuildin()
	{
		if (!sMainBuffer)
		{
			auto [flag1, frame_buffer] = create(TINY_FRAMEBUFFER_VIEWPORT);
			if (flag1 == FlagCreate::Succeeded)
			{
				auto [flag2, tex_color] = TextureManager::create2D(TINY_FRAMEBUFFER_VIEWPORT_COLOR_TEXTURE);
				if (flag2 == FlagCreate::Succeeded)
				{
					tex_color->setConfig(EngineContext::ScreenWidth, EngineContext::ScreenHeight
						, TextureInternalFormat::RGBA
						, TextureFormat::RGBA);
					tex_color->setAttachPosition(TextureAttachPosition::ColorComponent);
					frame_buffer->addAttachment(tex_color);
				}

				auto [flag3, tex_depth] = TextureManager::create2D(TINY_FRAMEBUFFER_VIEWPORT_DEPTH_TEXTURE);
				if (flag3 == FlagCreate::Succeeded)
				{
					tex_depth->setConfig(EngineContext::ScreenWidth, EngineContext::ScreenHeight
						, TextureInternalFormat::Depth
						, TextureFormat::Depth);
					tex_depth->setAttachPosition(TextureAttachPosition::DepthComponent);
					frame_buffer->addAttachment(tex_depth);
				}

				frame_buffer->generate();
				sMainBuffer = frame_buffer;
			}
		}

		return sMainBuffer;
	}

	FrameBuffer* FrameBufferManager::find(const std::string& name)
	{
		auto result = sUMap.find(name);
		if (result != sUMap.end())
		{
			return (*result).second;
		}

		return nullptr;
	}

	uint32_t FrameBufferManager::giveID()
	{
		uint32_t id = -1;

		if (sFreeIDs.size() > 0)
		{
			id = sFreeIDs.front();
			sFreeIDs.pop();
		}
		else
		{
			id = sID++;
		}

		return id;
	}

	bool FrameBufferManager::remove(std::string name)
	{
		auto it = sUMap.find(name);
		if (it != sUMap.end())
		{
			it->second->deleteObject();
			sUMap.erase(it);

			return true;
		}

		return false;
	}

}

