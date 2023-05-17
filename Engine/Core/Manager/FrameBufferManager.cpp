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
	FrameBuffer* FrameBufferCreator::create(const int& width, const int& height
		, const std::initializer_list<TextureInfo>& infos)
	{
		FrameBuffer* buffer = this->createFrameBuffer();
		buffer->beginBuild();
		for (auto& info : infos)
		{
			Texture* tex = nullptr;
			switch (info.type)
			{
			case TextureType::Texture2D:
				tex = TextureMgr::getInstance()->create2D(width, height, info);
				break;
			case TextureType::TextureCube:
				tex = TextureMgr::getInstance()->createCube(width, height, info);
				break;
			case TextureType::TextureRender2D:
				tex = TextureMgr::getInstance()->createRender2D(width, height, info);
				break;
			default:
				break;
			}

			buffer->attach(tex);
		}
		buffer->endBuild();

		return buffer;
	}

	FrameBuffer* FrameBufferCreator::create()
	{
		return this->createFrameBuffer();
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

	FrameBuffer* FrameBufferManager::create(const std::string& name
		, const int& width, const int& height
		, const std::initializer_list<TextureInfo>& infos)
	{
		auto result = mBufferUMap.tryEmplace(name, [&]()
		{
			return mCreator->create(width, height, infos);
		});
		return result.first->second;
	}

	FrameBuffer* FrameBufferManager::create(const std::string& name)
	{
		auto result = mBufferUMap.tryEmplace(name, [&]()
		{
			return mCreator->create();
		});
		return result.first->second;
	}

	FrameBuffer* FrameBufferManager::create()
	{
		return mCreator->create();
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

