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
#include "../Collision/BoundingBox.h"
#include "OctreeAgentList.h"
#include "OctreeConfig.h"

namespace tezcat::Tiny
{
	class OctreeAgent;
	class OctreeNode
	{
		friend class OctreeAgent;
	public:
		OctreeNode();
		OctreeNode(OctreeNode* parent, const OctPoint& point);

		void addAgent(OctreeAgent* agent);

		OctPoint getPoint()
		{
			return mOctPoint;
		}

		int getLayer()
		{
			return mLayerIndex;
		}

		void update()
		{

		}

		void log();

		void logBoundingBox();

		OctreeAgentList& getObjects() { return mObjects; }

	protected:
		OctPoint mOctPoint;
		int mLayerIndex;
		OctreeNode* mParent;
		BoundingBox mBoundingBox;
		OctreeNode* mChildren[8];
		OctreeAgentList mObjects;
	};

}
