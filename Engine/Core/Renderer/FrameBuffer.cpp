#include "FrameBuffer.h"
#include "../Renderer/Texture.h"

namespace tezcat::Tiny
{
	FrameBuffer* FrameBuffer::sDefaultBuffer;
	TinyStack<FrameBuffer*> FrameBuffer::sFrameBufferStack;

	TINY_RTTI_CPP(FrameBuffer);

	FrameBuffer::FrameBuffer()
		: mBufferID(-1)
		, mBuffers()
	{

	}

	FrameBuffer::~FrameBuffer()
	{

	}

	Texture* FrameBuffer::getBuffer(const int& index)
	{
		return mBuffers[index];
	}

	void FrameBuffer::bind(FrameBuffer* buffer)
	{
		buffer->bind();
		sFrameBufferStack.push(buffer);
	}

	void FrameBuffer::unbind(FrameBuffer* buffer)
	{
		if (sFrameBufferStack.top() != buffer)
		{
			throw std::invalid_argument("Unbind FrameBuffer Error!!! the buffer must be the same");
		}

		sFrameBufferStack.pop();
		sFrameBufferStack.top()->bind();
	}
}
