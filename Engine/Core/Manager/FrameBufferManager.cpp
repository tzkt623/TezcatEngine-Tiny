#include "FrameBufferManager.h"
#include "TextureManager.h"
#include "../Renderer/Texture.h"
#include "../Renderer/FrameBuffer.h"

namespace tezcat::Tiny
{
	//-------------------------------------------------
	//
	//	Creator
	//
	FrameBuffer* FrameBufferCreator::create(const int& width, const int& high, const std::initializer_list<TextureBufferInfo>& infos)
	{
		FrameBuffer* buffer = this->createFrameBuffer();
		buffer->build([&](FrameBuffer* self)
		{
			for (auto& info : infos)
			{
				auto tex = TextureMgr::getInstance()->createBuffer2D(width, high, info);
				self->attach(tex);
			}
		});

		return buffer;
	}

	FrameBuffer* FrameBufferCreator::createShadowMap(const int& width, const int& high)
	{
		FrameBuffer* buffer = this->createFrameBuffer();

		buffer->build([&](FrameBuffer* self)
		{
			auto tex = TextureMgr::getInstance()->createBuffer2D(width, high
				, TextureBufferInfo("ShadowMap"
					, TextureBufferType::DepthComponent
					, TextureChannel::Depth
					, TextureChannel::Depth
					, DataType::Float32
					, true));
			self->attach(tex);
		});

		return buffer;
	}


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

	FrameBuffer* FrameBufferManager::create(const std::string& name, const int& width, const int& high, const std::initializer_list<TextureBufferInfo>& infos)
	{
		auto result = this->find(name);
		if (result)
		{
			return result;
		}

		auto frame = mCreator->create(width, high, infos);
		this->addFrameBuffer(name, frame);
		return frame;
	}

	FrameBuffer* FrameBufferManager::createShadowMap(const std::string& name, const int& width, const int& high)
	{
		auto frame = mCreator->createShadowMap(width, high);
		this->addFrameBuffer(name, frame);
		return frame;
	}

	void FrameBufferManager::addFrameBuffer(const std::string& name, FrameBuffer* frameBuffer)
	{
		mBufferUMap.try_emplace(name, frameBuffer);
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
}

