#include "VertexBuffer.h"
#include "BaseGraphics.h"
#include "../Data/MeshData.h"

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
	TINY_OBJECT_CPP(IndexBuffer, IBuffer)
	IndexBuffer::IndexBuffer()
	{

	}

	IndexBuffer::~IndexBuffer()
	{
		Graphics::getInstance()->cmdDeleteIndexBuffer(mBufferID);
	}
}
