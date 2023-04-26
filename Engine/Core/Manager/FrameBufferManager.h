#pragma once

#include "Manager.h"
#include "../Tool/Singleton.h"
#include "../Head/TinyCpp.h"

namespace tezcat::Tiny
{
	class TextureRenderBuffer2D;
	struct TextureBufferInfo;

	class FrameBuffer;
	class TINY_API FrameBufferCreator
	{
	public:
		FrameBuffer* create(const int& width, const int& high, const std::initializer_list<TextureBufferInfo>& infos);
		FrameBuffer* createShadowMap(const int& width, const int& high);

	protected:
		virtual FrameBuffer* createFrameBuffer() = 0;
	};


	class TINY_API FrameBufferManager : public Manager<FrameBufferCreator>
	{
	public:
		FrameBufferManager();
		virtual ~FrameBufferManager();

		FrameBuffer* create(const std::string& name, const int& width, const int& high, const std::initializer_list<TextureBufferInfo>& infos);
		FrameBuffer* createShadowMap(const std::string& name, const int& width, const int& high);
		FrameBuffer* find(const std::string& name);

	private:
		void addFrameBuffer(const std::string& name, FrameBuffer* frameBuffer);

	private:
		//std::unordered_map<std::string, FrameBuffer*> mBufferUMap;

		TinyUMap<std::string, FrameBuffer*> mBufferUMap;

	};


	using FrameBufferMgr = SG<FrameBufferManager>;
}
