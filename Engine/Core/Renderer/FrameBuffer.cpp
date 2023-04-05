#include "FrameBuffer.h"


namespace tezcat::Tiny::Core
{
	FrameBuffer::FrameBuffer()
		: mBufferID(-1)
		, mBuffers()
	{

	}

	FrameBuffer::~FrameBuffer()
	{
	}

	TextureRenderBuffer2D* FrameBuffer::getBuffer(const int& index)
	{
		return mBuffers[index];
	}
}
