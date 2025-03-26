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

#include "Core/Base/TinyGC.h"
#include "Core/Base/TinyRefObject.h"
#include "Core/Base/TinyObject.h"
#include "Core/Debug/Debug.h"
#include "Core/Tool/StringTool.h"

namespace tezcat::Tiny
{
	std::queue<uint32_t> TinyGC::mFreeGCInfos;
	std::vector<TinyRefObject*> TinyGC::mMemoryPool;
	std::vector<TinyGCInfo*> TinyGC::mGCInfos =
	{
		new TinyGCInfo(0, -623, 0, nullptr)
	};

	void TinyGC::update()
	{
		if (!mMemoryPool.empty())
		{
			//auto size = mMemoryPool.size();
			//for (auto i = 0; i < size; i++)
			//{
			//	std::cerr << i << ":" << mMemoryPool[i]->getClassName() << "\n";
			//	mMemoryPool[i]->deleteObject();
			//}
			
			for (auto p : mMemoryPool)
			{
				//Log::engine(StringTool::stringFormat("Memory: {%s} RefSub: %d", s->getClassName().c_str(), s->getRefCount()));
				p->deleteObject();
			}

			mMemoryPool.clear();
		}
	}

	void TinyGC::manage(TinyRefObject* obj)
	{
		mMemoryPool.push_back(obj);
	}

	TinyGCInfo* TinyGC::getNextGCInfo(TinyRefObject* object)
	{
		if (mFreeGCInfos.empty())
		{
			auto info = new TinyGCInfo((TinyGCInfoID)mGCInfos.size(), 1, 0, object);
			mGCInfos.push_back(info);
			return info;
		}
		else
		{
			auto info = mGCInfos[mFreeGCInfos.front()];
			info->pointer = object;
			info->strongRef = 1;
			info->weakRef = 0;
			info->unique = false;
			mFreeGCInfos.pop();
			return info;
		}
	}

	void TinyGC::recycle(TinyGCInfo* info)
	{
		info->pointer = nullptr;
		info->strongRef = -1;
		info->weakRef = -1;
		mFreeGCInfos.push(info->index);
	}

}
