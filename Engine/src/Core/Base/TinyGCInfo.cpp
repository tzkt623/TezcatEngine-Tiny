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

#include "Core/Base/TinyGCInfo.h"
#include "Core/Base/TinyGC.h"

namespace tezcat::Tiny
{
	//TinyGCInfo* TinyGCInfo::Default = new TinyGCInfo(0, -9999999, -9999999, nullptr);
	const int32_t TinyGCInfo::EmptyStrongID = -623;

	TinyGCInfo::TinyGCInfo(TinyGCInfoID index, int32_t strongRef, int32_t weakRef, TinyRefObject* pointer)
		: index(index)
		, strongRef(strongRef)
		, weakRef(weakRef)
		, pointer(pointer)
		, unique(false)
	{

	}

// 	TinyGCInfo::TinyGCInfo(TinyGCInfo&& other) noexcept
// 	{
// 		this->pointer = other.pointer;
// 		this->strongRef = other.strongRef;
// 		this->weakRef = other.weakRef;
// 
// 		other.strongRef = -1;
// 		other.weakRef = -1;
// 		other.pointer = nullptr;
// 	}

	TinyGCInfo::~TinyGCInfo()
	{
		this->pointer = nullptr;
	}
}
