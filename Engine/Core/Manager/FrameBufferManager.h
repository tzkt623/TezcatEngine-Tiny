#pragma once

#include "Manager.h"
#include "../Tool/Singleton.h"
#include "../Head/TinyCpp.h"

namespace tezcat::Tiny
{
	class TextureRender2D;
	struct TextureInfo;
	struct TextureInfo;

	class FrameBuffer;
	class TINY_API FrameBufferCreator
	{
	public:
		FrameBuffer* create(const int& width
						  , const int& height
						  , const std::initializer_list<TextureInfo>& infos);

		FrameBuffer* create();
	protected:
		virtual FrameBuffer* createFrameBuffer() = 0;
	};


	class TINY_API FrameBufferManager : public Manager<FrameBufferCreator>
	{
	public:
		FrameBufferManager();
		virtual ~FrameBufferManager();


		/*
		* @author HCL
		* @info 2023|5|17
		* @brief 创建一个未配置的FB,但是不保存在Manager中
		*/
		FrameBuffer* create();

		/*
		* @author HCL
		* @info 2023|5|17
		* @brief 创建一个未配置的FB,并保存在Manager中
		*/
		FrameBuffer* create(const std::string& name);

		/*
		* @author HCL
		* @info 2023|5|17
		* @brief 创建一个配置的FB,并保存在Manager中
		*/
		FrameBuffer* create(const std::string& name
						  , const int& width
						  , const int& height
						  , const std::initializer_list<TextureInfo>& infos);


		FrameBuffer* find(const std::string& name);

	private:
		void addFrameBuffer(const std::string& name, FrameBuffer* frameBuffer);

	private:
		TinyUMap<std::string, FrameBuffer*> mBufferUMap;
	};


	using FrameBufferMgr = SG<FrameBufferManager>;
}
