#pragma once

/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

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

namespace tezcat::Tiny
{
	/*
	* 1.在shader中声明一个全局数据
	* 2.shader在生成时分析是否含有局数据
	*	查找当前全局数据是否已经注册
	*	如果没注册就注册一个,并记录在shader中
	*	如果注册了就找到对应的ID,并记录在对应的shader中
	*
	* 问题
	* 1.假如支持2个方向光
	* 
	*/

	class Texture2D;
	class TextureCube;
	class Shader;

	struct GlobalSlot
	{
		uint32_t index;

		virtual ~GlobalSlot() {}
	};

	struct GlobalSlot_Texture2D : public GlobalSlot
	{
		Texture2D* texture;

		virtual ~GlobalSlot_Texture2D() { texture = nullptr; }
	};

	class GlobalSlotManager
	{
		GlobalSlotManager() = delete;
		~GlobalSlotManager() = delete;

	public:

		static void registerSlot(GlobalSlot* slot)
		{
			slot->index = mSlots.size();
			mSlots.push_back(slot);
		}

		static GlobalSlot* getSlot(const uint32_t& index) { return mSlots[index]; }

		static void submit(Shader* shader);

	private:
		static std::vector<GlobalSlot*> mSlots;
	};
}
