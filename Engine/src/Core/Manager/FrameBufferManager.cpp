/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

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
	std::stack<FrameBuffer*> FrameBufferManager::sFrameBufferStack;
	std::unordered_map<std::string_view, FrameBuffer*> FrameBufferManager::sUMap;
	FrameBuffer* FrameBufferManager::sMainBuffer = nullptr;

	void FrameBufferManager::init()
	{
		sDefaultBuffer = FrameBuffer::create("Tiny_Default_FrameBuffer");
		sDefaultBuffer->saveObject();
		sFrameBufferStack.push(sDefaultBuffer);
	}

	std::tuple<bool, FrameBuffer*> FrameBufferManager::create(std::string name)
	{
		auto result = sUMap.find(name);
		if (result != sUMap.end())
		{
			return { false, result->second };
		}

		auto frame_buffer = FrameBuffer::create(name);
		frame_buffer->saveObject();
		sUMap.emplace(frame_buffer->getEngineName().toView(), frame_buffer);
		return { true, frame_buffer };
	}

	void FrameBufferManager::bind(FrameBuffer* buffer)
	{
		buffer->saveObject();
		sFrameBufferStack.push(buffer);
		Graphics::getInstance()->bind(buffer);
	}

	void FrameBufferManager::unbind(FrameBuffer* buffer)
	{
		if (sFrameBufferStack.top() != buffer)
		{
			throw std::invalid_argument("Unbind FrameBuffer Error!!! the buffer must be the same");
		}

		sFrameBufferStack.top()->deleteObject();

		if (sFrameBufferStack.empty())
		{
			return;
		}

		sFrameBufferStack.pop();
		Graphics::getInstance()->bind(sFrameBufferStack.top());
	}

	FrameBuffer* FrameBufferManager::getMainFrameBufferBuildin()
	{
		if (!sMainBuffer)
		{
			auto [flag1, frame_buffer] = create(TINY_FRAMEBUFFER_VIEWPORT);
			if (flag1)
			{
				auto [flag2, tex_color] = TextureManager::create2D(TINY_FRAMEBUFFER_VIEWPORT_COLOR_TEXTURE);
				if (flag2)
				{
					tex_color->setConfig(Engine::getScreenWidth(), Engine::getScreenHeight()
						, TextureInternalFormat::RGBA
						, TextureFormat::RGBA);
					tex_color->setAttachPosition(TextureAttachPosition::ColorComponent);
					frame_buffer->addAttachment(tex_color);
				}

				auto [flag3, tex_depth] = TextureManager::create2D(TINY_FRAMEBUFFER_VIEWPORT_DEPTH_TEXTURE);
				if (flag3)
				{
					tex_depth->setConfig(Engine::getScreenWidth(), Engine::getScreenHeight()
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

}

