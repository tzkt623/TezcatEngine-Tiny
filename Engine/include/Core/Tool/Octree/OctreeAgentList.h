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
#include "OctreeConfig.h"

namespace tezcat::Tiny
{
	class OctreeAgent;
	class OctreeAgentList
	{
	public:
		OctreeAgentList();
		~OctreeAgentList();

		void setRoot(OctreeAgent* root);
		void push(OctreeAgent* agent);

		void remove(OctreeAgent* agent);

		void foreach(const std::function<void(OctreeAgent*)>& function);

		void clear()
		{
			mRoot = nullptr;
		}

		uint32_t count()
		{
			return mCount;
		}

	private:
		OctreeAgent* mRoot = nullptr;
		uint32_t mCount = 0;
	};

}
