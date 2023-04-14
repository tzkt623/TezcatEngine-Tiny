#include "RenderObject.h"
#include "FrameBuffer.h"

namespace tezcat::Tiny::Core
{

	IRenderObserver::IRenderObserver()
		: mCullMask(0)
		, mFrameBuffer(FrameBuffer::getDefaultBuffer())
	{
		this->setCullLayer(0);
	}
}
