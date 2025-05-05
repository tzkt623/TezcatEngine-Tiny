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

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny
{

	/*
	 * 每一个物体同一时间只能存在一个Layer中
	 */
	class TINY_API LayerMask
	{
		LayerMask() {}
		~LayerMask() {}
	public:
		
		const uint32_t& getUID() { return mUID; }
		const std::string& getName() const
		{
			return sMaskWithName[mUID];
		}

	public:
		uint32_t mUID;

	public:
		static void init();

		static const std::string& getName(int index);
		static void setMask(int index, const std::string& maskName);

		static uint32_t getMask(const std::string& maskName);
		static uint32_t getMask(uint32_t index);

		static LayerMask* createMask(const std::string& name);

	private:
		static std::vector<std::string> sMaskWithName;
	};
}
