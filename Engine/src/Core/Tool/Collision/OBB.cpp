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
#include "Core/Tool/Collision/OBB.h"

namespace tezcat::Tiny
{
	OBB::OBB()
		: mMin(std::numeric_limits<float>::max())
		, mMax(std::numeric_limits<float>::min())
	{

	}

	OBB::OBB(const float3& min, const float3& max)
		: mMin(min)
		, mMax(max)
	{

	}

	OBB::OBB(const OBB& other)
		: mMax(other.mMax)
		, mMin(other.mMin)
	{

	}

	OBB::~OBB()
	{

	}

	float3 OBB::getCenter()
	{
		return float3((mMin.x + mMax.x) * 0.5f, (mMin.y + mMax.y) * 0.5f, (mMin.z + mMax.z) * 0.5f);
	}

	void OBB::getCorners(std::array<float3, 8>& array)
	{
		array[0] = { mMin.x, mMax.y, mMax.z };
		array[1] = { mMin.x, mMin.y, mMax.z };
		array[2] = { mMax.x, mMin.y, mMax.z };
		array[3] = { mMax.x, mMax.y, mMax.z };

		array[4] = { mMax.x, mMax.y, mMin.z };
		array[5] = { mMax.x, mMin.y, mMin.z };
		array[6] = { mMin.x, mMin.y, mMin.z };
		array[7] = { mMin.x, mMax.y, mMin.z };
	}

	bool OBB::impact(const OBB& other)
	{
		return ((mMin.x >= other.mMin.x) && (mMax.x <= other.mMax.x) || (other.mMin.x >= mMin.x) && (other.mMax.x <= mMax.x))
			&& ((mMin.y >= other.mMin.y) && (mMax.y <= other.mMax.y) || (other.mMin.y >= mMin.y) && (other.mMax.y <= mMax.y))
			&& ((mMin.z >= other.mMin.z) && (mMax.z <= other.mMax.z) || (other.mMin.z >= mMin.z) && (other.mMax.z <= mMax.z));
	}

	bool OBB::contain(const float3& point)
	{
		if (point.x < mMin.x) return false;
		if (point.y < mMin.y) return false;
		if (point.z < mMin.z) return false;
		if (point.x > mMax.x) return false;
		if (point.y > mMax.y) return false;
		if (point.z > mMax.z) return false;

		return true;
	}

	void OBB::combine(const OBB& other)
	{
		mMin = glm::min(mMin, other.mMin);
		mMax = glm::max(mMax, other.mMax);
	}

	void OBB::updateMinMax(const std::array<float3, 8>& array)
	{
		for (auto& i : array)
		{
			//
			if (i.x < mMin.x)
			{
				mMin.x = i.x;
			}

			if (i.x > mMax.x)
			{
				mMax.x = i.x;
			}

			//
			if (i.y < mMin.y)
			{
				mMin.y = i.y;
			}

			if (i.y > mMax.y)
			{
				mMax.y = i.y;
			}

			//
			if (i.z < mMin.z)
			{
				mMin.z = i.z;
			}

			if (i.z > mMax.z)
			{
				mMax.z = i.z;
			}
		}
	}

	void OBB::transform(const float4x4& localToWorldMatrix)
	{
		std::array<float3, 8> array =
		{
			localToWorldMatrix * float4(mMin.x, mMax.y, mMax.z, 1.0f),
			localToWorldMatrix * float4(mMin.x, mMin.y, mMax.z, 1.0f),
			localToWorldMatrix * float4(mMax.x, mMin.y, mMax.z, 1.0f),
			localToWorldMatrix * float4(mMax.x, mMax.y, mMax.z, 1.0f),

			localToWorldMatrix * float4(mMax.x, mMax.y, mMin.z, 1.0f),
			localToWorldMatrix * float4(mMax.x, mMin.y, mMin.z, 1.0f),
			localToWorldMatrix * float4(mMin.x, mMin.y, mMin.z, 1.0f),
			localToWorldMatrix * float4(mMin.x, mMax.y, mMin.z, 1.0f)
		};

		this->updateMinMax(array);
	}

	void OBB::reset()
	{
		mMin = float3(std::numeric_limits<float>::max());
		mMax = float3(std::numeric_limits<float>::min());
	}

	bool OBB::isValid()
	{
		return !(mMin.x > mMax.x || mMin.y > mMax.y || mMin.z > mMax.z);
	}
}
