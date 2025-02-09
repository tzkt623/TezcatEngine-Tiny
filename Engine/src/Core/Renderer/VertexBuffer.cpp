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
#include "Core/Data/MeshData.h"

namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(IBuffer, TinyObject)


	IBuffer::IBuffer()
		: mBufferID(0)
		, mData(nullptr)
		, mDataSize(0)
	{

	}

	IBuffer::~IBuffer()
	{

	}



	//---------------------------------------------
	//
	//
	//
	TINY_OBJECT_CPP(VertexBuffer, IBuffer)
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
	TINY_OBJECT_CPP(IndexBuffer, IBuffer)
	IndexBuffer::IndexBuffer()
	{

	}

	IndexBuffer::~IndexBuffer()
	{
		Graphics::getInstance()->deleteBuffer(this);
		//Graphics::getInstance()->cmdDeleteIndexBuffer(mBufferID);
	}
}
