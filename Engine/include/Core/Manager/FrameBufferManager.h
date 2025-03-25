#pragma once
/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "../Head/TinyCpp.h"
#include "../Head/FlagConfig.h"

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

		static uint32_t giveID();

		static void recycleID(uint32_t id)
		{
			sFreeIDs.push(id);
		}

	public:
		static void init();
		static const std::unordered_map<std::string_view, FrameBuffer*>& getAllFrameBuffer() { return sUMap; }
		static std::tuple<FlagCreate, FrameBuffer*> create(std::string name);
		static bool remove(std::string name);

		static FrameBuffer* getMainFrameBufferBuildin();
		static FrameBuffer* find(const std::string& name);

	public:
		static void bind(FrameBuffer* buffer);
		static void unbind(FrameBuffer* buffer);
		static FrameBuffer* getDefaultBuffer() { return sDefaultBuffer; }

	private:
		static std::unordered_map<std::string_view, FrameBuffer*> sUMap;
		static uint32_t sID;
		static std::queue<uint32_t> sFreeIDs;
		static std::stack<FrameBuffer*> sFrameBufferStack;
		static FrameBuffer* sDefaultBuffer;

		static FrameBuffer* sMainBuffer;
	};

constexpr auto TINY_FRAMEBUFFER_VIEWPORT = "Tiny_Viewport_FrameBuffer";
constexpr auto TINY_FRAMEBUFFER_VIEWPORT_COLOR_TEXTURE = "Tiny_Viewport_ColorBuffer";
constexpr auto TINY_FRAMEBUFFER_VIEWPORT_DEPTH_TEXTURE = "Tiny_Viewport_DepthBuffer";
}
