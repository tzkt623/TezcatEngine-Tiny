#include "FrameBufferManager.h"

namespace tezcat::Tiny::Core
{
	FrameBufferManager::FrameBufferManager()
	{
		SG<FrameBufferManager>::attach(this);
	}

	FrameBufferManager::~FrameBufferManager()
	{
	}

	FrameBuffer* FrameBufferManager::create(const int& width, const int& high, const std::initializer_list<TextureBufferInfo>& infos)
	{
		return mCreator->create(width, high, infos);
	}
}

