#pragma once

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
