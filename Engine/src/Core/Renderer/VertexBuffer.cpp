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

#include "Core/Renderer/VertexBuffer.h"
#include "Core/Renderer/BaseGraphics.h"
#include "Core/Renderer/RenderCommand.h"
#include "Core/Data/MeshData.h"
#include "Core/Manager/VertexBufferManager.h"



namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(IBuffer, TinyObject);

	IBuffer::IBuffer()
		: mBufferID(0)
		, mData(nullptr)
		, mDataSize(0)
		, mGenerated(false)
	{

	}

	IBuffer::~IBuffer()
	{
		free(mData);
	}

	void IBuffer::init(const size_t& dataSize, const void* data /*= nullptr*/)
	{
		auto temp = realloc(mData, dataSize);
		if (temp)
		{
			mDataSize = dataSize;
			mData = temp;
			memcpy_s(mData, mDataSize, data, dataSize);
		}
	}

	void IBuffer::updateData(const void* data, const size_t& dataSize)
	{
		auto temp = realloc(mData, dataSize);
		if (temp)
		{
			mDataSize = dataSize;
			mData = temp;
			memcpy_s(mData, mDataSize, data, dataSize);
		}
	}

	void IBuffer::resetSize(size_t size)
	{
		auto temp = realloc(mData, size);
		if (temp)
		{
			mDataSize = size;
			mData = temp;
			memset(mData, 0, mDataSize);
		}
	}

	void IBuffer::copy(const void* data, const size_t& dataSize, const size_t& offset)
	{
		memcpy_s((uint8_t*)mData + offset, mDataSize - offset, data, dataSize);
	}


	//---------------------------------------------
	//
	//
	//
	TINY_OBJECT_CPP(VertexBuffer, IBuffer);
	VertexBuffer::VertexBuffer()
	{

	}

	VertexBuffer::~VertexBuffer()
	{
		Graphics::getInstance()->deleteBuffer(this);
	}

	void VertexBuffer::setLayoutData(VertexPosition position, VertexLayoutType type)
	{
		mLayoutData.position = position;
		mLayoutData.type = type;
		mLayoutData.stride = VertexLayout::getTypeSize(type);
	}

	//---------------------------------------------
	//
	//
	//
	TINY_OBJECT_CPP(IndexBuffer, IBuffer);
	IndexBuffer::IndexBuffer()
	{

	}

	IndexBuffer::~IndexBuffer()
	{
		Graphics::getInstance()->deleteBuffer(this);
		//Graphics::getInstance()->cmdDeleteIndexBuffer(mBufferID);
	}


	TINY_OBJECT_CPP(UniformBuffer, IBuffer);
	UniformBuffer::UniformBuffer()
		: mName()
	{

	}

	UniformBuffer::~UniformBuffer()
	{

	}

	void UniformBuffer::update(const uint32_t& index, const void* data, const size_t& dataSize)
	{
		auto& layout = mLayout->mSlot[index];
		if (layout.offset < 0)
		{
			return;
		}
		memcpy((uint8_t*)mData + layout.offset, data, dataSize);
	}

	void UniformBuffer::setLayout(const std::string_view& layoutName, const std::function<void(UniformBufferLayout*)>& function)
	{
		auto [flag, layout] = VertexBufferManager::createUniformBufferLayout(layoutName.data());

		if (!layout->addHolder(this))
		{
			return;
		}

		mLayout = layout;
		if (!mLayout->isLayoutChecked())
		{
			function(mLayout.get());
			mLayout->layoutCheckComplete();
		}

		this->resetSize(mLayout->mSize);

		if (mLayout->isValied())
		{
			Graphics::getInstance()->addCommand<RenderCMD_CreateUniformBuffer>(this, mLayout->mBindingIndex);
		}
	}

	void UniformBuffer::updateLayoutData()
	{
		if (mLayout->isValied())
		{
			this->resetSize(mLayout->mSize);
			if (mOnLayoutDataUpdated)
			{
				mOnLayoutDataUpdated(this);
			}
			Graphics::getInstance()->addCommand<RenderCMD_CreateUniformBuffer>(this, mLayout->mBindingIndex);
		}
	}

	void UniformBuffer::onClose()
	{
		mLayout->removeHolder(this);
	}
}
