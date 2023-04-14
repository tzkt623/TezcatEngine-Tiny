#include "FrameBuffer.h"

namespace tezcat::Tiny::Core
{
	FrameBuffer* FrameBuffer::sDefaultBuffer;
	std::stack<FrameBuffer*> FrameBuffer::sFrameBufferStack;

	FrameBuffer::FrameBuffer()
		: mBufferID(-1)
		, mBuffers()
	{

	}

	FrameBuffer::~FrameBuffer()
	{
		mBuffers.clear();
	}

	TextureRenderBuffer2D* FrameBuffer::getBuffer(const int& index)
	{
		return mBuffers[index];
	}

	void FrameBuffer::bind(FrameBuffer* buffer)
	{
		sFrameBufferStack.push(buffer);
		buffer->bind();
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
