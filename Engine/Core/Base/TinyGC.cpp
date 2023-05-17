#include "TinyGC.h"
#include "TinyRefObject.h"
#include "TinyObject.h"
#include "../Tool/Log.h"
#include "../Tool/StringTool.h"

namespace tezcat::Tiny
{
	std::deque<uint32_t> TinyGC::mFreeGCInfos;
	std::vector<TinyRefObject*> TinyGC::mMemoryPool;
	std::vector<TinyGCInfo*> TinyGC::mGCInfos =
	{
		new TinyGCInfo(0, -623, 0, nullptr)
	};

	TinyGCInfo* TinyGC::DefaultGCInfo = TinyGC::mGCInfos[0];


	void TinyGC::update()
	{
		if (mMemoryPool.empty())
		{
			return;
		}

		for (size_t i = 0; i < mMemoryPool.size(); i++)
		{
			auto s = mMemoryPool[i];
			Log::engine(StringTool::stringFormat("Memory: {%s} RefSub: %d", s->getClassName().c_str(), s->getRefCount()));
			s->subRef();
		}

		mMemoryPool.clear();
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
			auto index = mFreeGCInfos.front();
			mFreeGCInfos.pop_front();
			auto info = mGCInfos[index];
			info->pointer = object;
			info->strongRef = 1;
			info->weakRef = 0;
			return info;
		}
	}

	void TinyGC::recycle(TinyGCInfo* info)
	{
		info->pointer = nullptr;
		info->strongRef = -1;
		info->weakRef = -1;
		mFreeGCInfos.push_back(info->index);
	}
}

