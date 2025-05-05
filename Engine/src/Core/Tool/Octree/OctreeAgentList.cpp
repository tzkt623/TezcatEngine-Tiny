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

#include "Core/Tool/Octree/OctreeAgentList.h"
#include "Core/Component/OctreeAgent.h"

namespace tezcat::Tiny
{
	OctreeAgentList::OctreeAgentList()
	{

	}

	OctreeAgentList::~OctreeAgentList()
	{
		mRoot = nullptr;
	}

	void OctreeAgentList::push(OctreeAgent* agent)
	{
		if (mRoot == nullptr)
		{
			mRoot = agent;
		}
		else
		{
			mRoot->preAgent = agent;
			agent->nextAgent = mRoot;
			mRoot = agent;
		}

		++mCount;
	}

	void OctreeAgentList::remove(OctreeAgent* agent)
	{
		--mCount;
	}

	void OctreeAgentList::foreach(const std::function<void(OctreeAgent*)>& function)
	{
		int count = 0;
		OctreeAgent* agent = mRoot;
		while (agent)
		{
			function(agent);
			agent = agent->nextAgent;
			count++;
		}

		if (count != mCount)
		{
			throw new std::logic_error("");
		}
	}

	void OctreeAgentList::setRoot(OctreeAgent* root)
	{
		mRoot = root;
	}



}
