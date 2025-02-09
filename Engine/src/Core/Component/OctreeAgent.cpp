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

#include "Core/Component/OctreeAgent.h"
#include "Core/Tool/Octree/OctreeAgentList.h"
#include "Core/Tool/Octree/OctreeNode.h"

namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(OctreeAgent, ComponentT<OctreeAgent>)
	OctreeAgent::OctreeAgent()
	{

	}

	OctreeAgent::~OctreeAgent()
	{
		this->preAgent = nullptr;
		this->nextAgent = nullptr;
		this->hostNode = nullptr;
	}

	void OctreeAgent::update()
	{
		// 	if (this->hostNode->mBoundingBox.contains(boundingBox))
		// 	{
		// 		return;
		// 	}

			//包不住了,删除自己
		this->removeSelf();

		//往回找,找到能包住自己的parent
		OctreeNode* current = this->hostNode;
		OctreeNode* temp = nullptr;
		this->hostNode = nullptr;
		//节点不能为空
		while (current)
		{
			//找到了
			if (current->mBoundingBox.contains(boundingBox))
			{
				//加进去
				current->addAgent(this);
				break;
			}

			temp = current;
			current = current->mParent;
		}

		if (this->hostNode == nullptr)
		{
			throw std::logic_error("");
		}
	}

	void OctreeAgent::removeSelf()
	{
		if (this->preAgent)
		{
			this->preAgent->nextAgent = this->nextAgent;

			if (this->nextAgent)
			{
				this->nextAgent->preAgent = this->preAgent;
			}
		}
		else
		{
			//没有值说明是root
			//把后面一个变成root
			if (this->nextAgent)
			{
				this->nextAgent->preAgent = nullptr;
			}
			this->hostNode->mObjects.setRoot(this->nextAgent);
		}

		this->hostNode->mObjects.remove(this);
		this->preAgent = nullptr;
		this->nextAgent = nullptr;
	}

	void OctreeAgent::forearchPossible(const std::function<void(OctreeAgent*)>& function)
	{
		OctreeNode* current = this->hostNode;
		//父节点不能为空
		while (current)
		{
			current->mObjects.foreach(function);
			current = current->mParent;
		}
	}
}
