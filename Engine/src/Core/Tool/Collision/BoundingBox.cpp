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

#include "Core/Tool/Collision/BoundingBox.h"

namespace tezcat::Tiny
{
#define fmin std::numeric_limits<float>::min()
#define fmax std::numeric_limits<float>::max()

	BoundingBox::BoundingBox(const float3& min, const float3& max)
		: min(min)
		, max(max)
	{

	}

	BoundingBox::BoundingBox()
		: min(fmax)
		, max(fmin)
	{

	}

	void BoundingBox::collect(const BoundingBox& other)
	{
		if (min.x > other.min.x)
		{
			min.x = other.min.x;
		}

		if (min.y > other.min.y)
		{
			min.y = other.min.y;
		}

		if (min.z > other.min.z)
		{
			min.z = other.min.z;
		}

		if (max.x < other.max.x)
		{
			max.x = other.max.x;
		}

		if (max.y < other.max.y)
		{
			max.y = other.max.y;
		}

		if (max.z < other.max.z)
		{
			max.z = other.max.z;
		}
	}

	void BoundingBox::collect(const float3& point)
	{
		if (min.x > point.x)
		{
			min.x = point.x;
		}

		if (min.y > point.y)
		{
			min.y = point.y;
		}

		if (min.z > point.z)
		{
			min.z = point.z;
		}

		if (max.x < point.x)
		{
			max.x = point.x;
		}

		if (max.y < point.y)
		{
			max.y = point.y;
		}

		if (max.z < point.z)
		{
			max.z = point.z;
		}
	}

	bool BoundingBox::contains(const BoundingBox& other)
	{
		if ((min.x <= other.min.x && min.y <= other.min.y && min.z <= other.min.z)
			&& (max.x >= other.max.x && max.y >= other.max.y && max.z >= other.max.z))
		{
			return true;
		}

		return false;
	}

	bool BoundingBox::impact(const BoundingBox& other)
	{
		if (min.x > other.max.x || min.y > other.max.y || min.z > other.max.z
			|| max.x < other.min.x || max.y < other.min.y || max.z < other.min.z)
		{
			return false;
		}

		return true;
	}

	void BoundingBox::reset(const float3& min, const float3& max)
	{
		this->min = min;
		this->max = max;
	}

}

