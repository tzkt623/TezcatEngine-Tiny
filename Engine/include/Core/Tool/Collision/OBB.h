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
#include "Core/Head/ConfigHead.h"
#include "Core/Head/GLMHead.h"
#include "Core/Head/TinyCpp.h"

namespace tezcat::Tiny
{
	class TINY_API OBB
	{
	public:
		OBB();
		OBB(const float3& min, const float3& max);
		OBB(const OBB& other);

		virtual ~OBB();

		float3 getCenter();
		void getCorners(std::array<float3, 8>& array);
		bool impact(const OBB& other);
		bool contain(const float3& point);
		void combine(const OBB& other);
		void updateMinMax(const std::array<float3, 8>& array);
		void transform(const float4x4& localToWorldMatrix);
		void reset();
		bool isValid();

	private:
		float3 mMin;
		float3 mMax;
	};
}
