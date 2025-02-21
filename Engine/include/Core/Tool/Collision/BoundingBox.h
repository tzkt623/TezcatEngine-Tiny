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
#include "Core/Head/ConfigHead.h"
#include "Core/Head/GLMHead.h"

namespace tezcat::Tiny
{
	class TINY_API BoundingBox
	{
	public:
		BoundingBox();
		BoundingBox(const float3& min, const float3& max);

		void reset(const float3& min, const float3& max);

		bool contains(const BoundingBox& other);

		bool impact(const BoundingBox& other);

		void translate(const float3& offset)
		{
			this->min += offset;
			this->max += offset;
		}

		void setPosition(float3& position)
		{
			auto d = this->getDimensions() * 0.5f;
			this->min = position - d;
			this->max = position + d;
		}

		void collect(const BoundingBox& other);
		void collect(const float3& point);

		float3 getDimensions() { return max - min; }
		float3 getPosition() { return (this->min + this->max) * 0.5f; }


		float3 max;
		float3 min;

	};

}
