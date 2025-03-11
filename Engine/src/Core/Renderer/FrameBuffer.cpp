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

#include "Core/Renderer/FrameBuffer.h"
#include "Core/Renderer/Texture.h"
#include "Core/Renderer/BaseGraphics.h"
#include "Core/Renderer/Pipeline.h"
#include "Core/Renderer/RenderCommand.h"

#include "Core/Manager/FrameBufferManager.h"


namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(FrameBuffer, TinyObject)

	FrameBuffer::FrameBuffer()
		: mBufferID(0)
		, mComponents()
		, mName()
		, mUID(FrameBufferManager::giveID())
		, mCurrentFrame(Pipeline::getFrameCount() - 1)
	{

	}

	FrameBuffer::FrameBuffer(std::string name)
		: mBufferID(0)
		, mComponents()
		, mName(std::move(name))
		, mUID(FrameBufferManager::giveID())
		, mCurrentFrame(Pipeline::getFrameCount() - 1)
	{
		mEngineName = mName;
	}

	FrameBuffer::~FrameBuffer()
	{
		FrameBufferManager::recycleID(mUID);
	}

	void FrameBuffer::generate()
	{
		Graphics::getInstance()->addCommand(new RenderCMD_CreateFrameBuffer(this));
	}

	void FrameBuffer::addAttachment(Texture* tex)
	{
		tex->saveObject();
		mComponents.push_back(tex);
	}

	std::string FrameBuffer::getMemoryInfo()
	{
		return TINY_OBJECT_MEMORY_INFO();
	}

}
