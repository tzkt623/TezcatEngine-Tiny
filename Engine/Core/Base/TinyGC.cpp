#include "TinyGC.h"
#include "TinyRefObject.h"
#include "TinyObject.h"
#include "../Tool/Log.h"
#include "../Tool/StringTool.h"

namespace tezcat::Tiny
{
	std::queue<uint32> TinyGC::mFreeGCInfos;
	std::vector<TinyRefObject*> TinyGC::mMemoryPool;
	std::vector<TinyGCInfo*> TinyGC::mGCInfos =
	{
		new TinyGCInfo(0, -623, 0, nullptr)
	};

	void TinyGC::update()
	{
		if (!mMemoryPool.empty())
		{
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
