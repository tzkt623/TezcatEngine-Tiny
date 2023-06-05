#pragma once

#include "Manager.h"
#include "../Tool/Singleton.h"
#include "../Head/TinyCpp.h"

namespace tezcat::Tiny
{
	class FrameBuffer;
	/*
	* FrameBufferManager
	* 
	* @brief 帧缓冲管理器,负责创建,保存,查找
	*/
	class TINY_API FrameBufferManager
	{
	public:
		FrameBufferManager();
		virtual ~FrameBufferManager();

		FrameBuffer* create(const std::string& name);

		FrameBuffer* find(const std::string& name);
		void add(const std::string& name, FrameBuffer* frameBuffer);

	private:
		TinyUMap<std::string, FrameBuffer*> mBufferUMap;
	};


	using FrameBufferMgr = SG<FrameBufferManager>;
}
