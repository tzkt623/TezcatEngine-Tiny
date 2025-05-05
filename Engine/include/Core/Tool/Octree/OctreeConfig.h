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
#include "Core/Head/TinyCpp.h"

namespace tezcat::Tiny
{
	using OctPointType = unsigned char;

	enum class OctPoint : OctPointType
	{
		Root = 0,
		NF1 = 1 << 0,
		NF2 = 1 << 1,
		NF3 = 1 << 2,
		NF4 = 1 << 3,
		NB1 = 1 << 4,
		NB2 = 1 << 5,
		NB3 = 1 << 6,
		NB4 = 1 << 7,
	};

	struct OctreeConfig
	{
		static int SplitCount;
		static float MinBounds;
		static int ChildrenCount;
		static std::string getName(OctPoint point)
		{
			switch (point)
			{
			case OctPoint::Root:return "Root";
			case OctPoint::NF1:return "NF1";
			case OctPoint::NF2:return "NF2";
			case OctPoint::NF3:return "NF3";
			case OctPoint::NF4:return "NF4";
			case OctPoint::NB1:return "NB1";
			case OctPoint::NB2:return "NB2";
			case OctPoint::NB3:return "NB3";
			case OctPoint::NB4:return "NB4";
			default: return "Error";
			}
		}
	};

}
