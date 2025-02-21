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
#include "OctreeNode.h"
#include "../Collision/BoundingBox.h"

namespace tezcat::Tiny
{
	class Octree : public OctreeNode
	{
		/*
		* 0层:1(8^0)
		* 1层:8(8^1)
		* 2层:8*8(8^2)
		* 3层:8*8*8(8^3)
		* 4层:8*8*8*8(8^4)
		* N层:8^n
		* 总计:8^0+...+8^n
		*/

	public:
		void update();

		void setBoundingBox(const glm::vec3& min, const glm::vec3& max)
		{
			mBoundingBox.reset(min, max);
		}

		void collectBoundingBox(const BoundingBox& other)
		{
			mBoundingBox.collect(other);
		}
	};
}
