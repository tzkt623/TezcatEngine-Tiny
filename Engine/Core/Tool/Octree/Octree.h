#pragma once
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
