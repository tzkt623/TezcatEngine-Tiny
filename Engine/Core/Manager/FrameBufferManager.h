#pragma once

#include "Manager.h"
#include "Utility/Singleton.h"
#include "../Head/CppHead.h"

using namespace tezcat::Tiny::Utility;
namespace tezcat::Tiny::Core
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
		FrameBuffer* addFrameBuffer(const std::string& name, FrameBuffer* frameBuffer);

	private:
		std::unordered_map<std::string, FrameBuffer*> mBufferUMap;
	};


	using FrameBufferMgr = SG<FrameBufferManager>;
}
