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
#include "../Head/ConfigHead.h"
#include "../Head/CppHead.h"

namespace tezcat::Tiny
{
	using TinyGCInfoID = int32_t;

	class TinyRefObject;
	struct TINY_API TinyGCInfo
	{
		TinyGCInfoID index = 0;
		int32_t strongRef = 0;
		int32_t weakRef = 0;
		bool unique = false;
		TinyRefObject* pointer = nullptr;

		TinyGCInfo() {}
		TinyGCInfo(TinyGCInfoID index, int32_t strongRef, int32_t weakRef, TinyRefObject* pointer);
		//TinyGCInfo(TinyGCInfo&& other) noexcept;

		~TinyGCInfo();


		//static TinyGCInfo* Default;

		const static int32_t EmptyStrongID;
	};
}
