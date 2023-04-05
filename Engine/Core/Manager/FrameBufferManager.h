#pragma once

#include "Manager.h"
#include "Utility/Singleton.h"
#include "../Head/CppHead.h"

using namespace tezcat::Tiny::Utility;
namespace tezcat::Tiny::Core
{
	class FrameBuffer;
	struct TextureBufferInfo;
	class TINY_API FrameBufferCreator
	{
	public:
		virtual FrameBuffer* create(const int& width, const int& high, const std::initializer_list<TextureBufferInfo>& infos) = 0;
	};


	class TINY_API FrameBufferManager : public Manager<FrameBufferCreator>
	{
	public:
		FrameBufferManager();
		virtual ~FrameBufferManager();

		FrameBuffer* create(const int& width, const int& high, const std::initializer_list<TextureBufferInfo>& infos);

	private:
		std::unordered_map<std::string, FrameBuffer*> mBufferUMap;
	};


	using FrameBufferMgr = SG<FrameBufferManager>;
}
