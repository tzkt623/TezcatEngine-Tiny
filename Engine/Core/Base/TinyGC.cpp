#include "TinyGC.h"
#include "TinyRefObject.h"
#include "TinyObject.h"
#include "../Tool/Log.h"
#include "../Tool/StringTool.h"

namespace tezcat::Tiny
{
	TinyGC TinyGC::sInstance;

	void TinyGC::update()
	{
		if (mMemoryPool.empty())
		{
			return;
		}

		for (auto s : mMemoryPool)
		{
			Log::engine(StringTool::stringFormat("Memory: {%s} RefSub: %d", s->getClassName().c_str(), s->getRefCount()));
			s->subRef();
		}

		mMemoryPool.clear();
	}

	void TinyGC::manage(TinyRefObject* obj)
	{
		mMemoryPool.push_back(obj);
	}

	TinyGCInfo* TinyGC::getNextGCInfo()
	{
		if (mFreeGCInfos.empty())
		{
			return &mGCInfos.emplace_back(mGCInfos.size());
		}
		else
		{
			auto index = mFreeGCInfos.top();
			mFreeGCInfos.pop();
			return &mGCInfos[index];
		}
	}

	void TinyGC::collect(const TinyGCInfo* info)
	{
		if (info->strongRef < 1 && info->weakRef < 1)
		{
			mFreeGCInfos.push(info->index);
		}
	}

	TinyGC::TinyGC()
		: mGCInfos()
	{
		mGCInfos.reserve(65535);
	}

}

