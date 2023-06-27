#include "VertexBuffer.h"
#include "BaseGraphics.h"
#include "../Data/MeshData.h"

namespace tezcat::Tiny
{
	//---------------------------------------------
	//
	//
	//
	TINY_RTTI_CPP(VertexBuffer);
	VertexBuffer::VertexBuffer()
	{

	}

	VertexBuffer::~VertexBuffer()
	{
		Graphics::getInstance()->cmdDeleteVertexBuffer(mBufferID);
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
	TINY_RTTI_CPP(IndexBuffer);
	IndexBuffer::IndexBuffer()
	{

	}

	IndexBuffer::~IndexBuffer()
	{
		Graphics::getInstance()->cmdDeleteIndexBuffer(mBufferID);
	}
}
