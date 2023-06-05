#include "FrameBufferManager.h"
#include "TextureManager.h"
#include "../Renderer/Texture.h"
#include "../Renderer/FrameBuffer.h"

namespace tezcat::Tiny
{
	//-------------------------------------------------
	//
	//	Manager
	//
	FrameBufferManager::FrameBufferManager()
	{
		SG<FrameBufferManager>::attach(this);
	}

	FrameBufferManager::~FrameBufferManager()
	{

	}

	void FrameBufferManager::add(const std::string& name, FrameBuffer* frameBuffer)
	{
		frameBuffer->addRef();
		auto result = mBufferUMap.try_emplace(name, frameBuffer);
		if (!result.second)
		{
			result.first->second->subRef();
			result.first->second = frameBuffer;
		}
	}

	FrameBuffer* FrameBufferManager::find(const std::string& name)
	{
		auto it = mBufferUMap.find(name);
		if (it != mBufferUMap.end())
		{
			return it->second;
		}

		return nullptr;
	}

	FrameBuffer* FrameBufferManager::create(const std::string& name)
	{
		auto result = mBufferUMap.try_emplace(name, nullptr);
		if (result.second)
		{
			auto fb = FrameBuffer::create(name);
			fb->addRef();
			result.first->second = fb;
		}

		return result.first->second;
	}

}

