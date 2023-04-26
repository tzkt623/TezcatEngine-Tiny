#include "TinyRefObject.h"
#include "TinyGC.h"

namespace tezcat::Tiny
{
	TinyGCInfo* TinyGCInfo::Default = new TinyGCInfo(-9999999, -9999999, nullptr);

	void TinyGCInfo::collect()
	{
		TinyGC::getInstance().collect(this);
	}


	const std::string TinyRefObject::Empty = "TinyRefObject";
	uint32_t TinyRefObject::sIDGiver = 0;
	std::deque<uint32_t> TinyRefObject::sFreeUIDPool;

	void TinyRefObject::manageThis()
	{
		TinyGC::getInstance().manage(this);
	}

	TinyRefObject::TinyRefObject()
		: mGCInfo(new TinyGCInfo(1, 0, this))
	{
		if (sFreeUIDPool.empty())
		{
			mUID = sIDGiver++;
		}
		else
		{
			mUID = sFreeUIDPool.front();
			sFreeUIDPool.pop_front();
		}
	}

	TinyRefObject::~TinyRefObject()
	{
		mGCInfo = nullptr;
		sFreeUIDPool.push_back(mUID);
	}

	void TinyRefObject::subRef()
	{
		if (--mGCInfo->strongRef < 1)
		{
			if (mGCInfo->weakRef < 1)
			{
				delete mGCInfo;
				mGCInfo = nullptr;
			}

			//mGCInfo->collect();
			delete this;
		}
	}



	TinyBaseWeakRef::~TinyBaseWeakRef()
	{
		if (mGCInfo == TinyGCInfo::Default)
		{
			return;
		}

		this->release();
	}

	void TinyBaseWeakRef::release()
	{
		if (--(mGCInfo->weakRef) < 1)
		{
			if (mGCInfo->strongRef < 1)
			{
				delete mGCInfo;
				mGCInfo = nullptr;
			}
		}
	}
}

