#pragma once

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
		friend class FrameBuffer;
		FrameBufferManager() = delete;
		~FrameBufferManager() = delete;

	private:

		static uint32 giveID()
		{
			uint32 id = -1;

			if (sFreeIDs.size() > 0)
			{
				id = sFreeIDs.front();
				sFreeIDs.pop();
			}
			else
			{
				id = sID++;
			}

			return id;
		}

		static void recycleID(uint32 id)
		{
			sFreeIDs.push(id);
		}

		static void store(const std::string&name, FrameBuffer* buffer)
		{
			auto it = sUMap.try_emplace(name, buffer);
			if (!it.second)
			{
				TINY_THROW_LOGIC(fmt::format("FrameBuffer {0} had stored!", name));
			}
		}

	public:
		static void init();
		static const std::unordered_map<std::string_view, FrameBuffer*>& getAllFrameBuffer() { return sUMap; }
		static std::tuple<bool, FrameBuffer*> create(std::string name);

		static FrameBuffer* getMainFrameBufferBuildin();

		static FrameBuffer* find(const std::string& name)
		{
			auto result = sUMap.find(name);
			if (result != sUMap.end())
			{
				return (*result).second;
			}

			return nullptr;
		}

	public:
		static void bind(FrameBuffer* buffer);
		static void unbind(FrameBuffer* buffer);
		static FrameBuffer* getDefaultBuffer() { return sDefaultBuffer; }

	private:
		static std::unordered_map<std::string_view, FrameBuffer*> sUMap;
		static uint32 sID;
		static std::queue<uint32> sFreeIDs;
		static std::stack<FrameBuffer*> sFrameBufferStack;
		static FrameBuffer* sDefaultBuffer;

		static FrameBuffer* sMainBuffer;
	};

constexpr auto TINY_FRAMEBUFFER_VIEWPORT = "Viewport_FrameBuffer";
constexpr auto TINY_FRAMEBUFFER_VIEWPORT_COLOR_TEXTURE = "Viewport_ColorBuffer";
constexpr auto TINY_FRAMEBUFFER_VIEWPORT_DEPTH_TEXTURE = "Viewport_DepthBuffer";
}
