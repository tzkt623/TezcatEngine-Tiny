#include "TinyRefObject.h"
#include "TinyGC.h"

namespace tezcat::Tiny
{
	//-----------------------------------------------------------------
	//
	//	TinyRefObject
	//
	const std::string TinyRefObject::Empty = "TinyRefObject";

	void TinyRefObject::manageThis()
	{
		TinyGC::manage(this);
	}

	TinyRefObject::TinyRefObject()
		: mGCInfo(TinyGC::getNextGCInfo(this))
	{

	}

	TinyRefObject::~TinyRefObject()
	{
		mGCInfo = nullptr;
	}

	void TinyRefObject::subRef()
	{
		if (--mGCInfo->strongRef < 1)
		{
			if (mGCInfo->weakRef < 1)
			{
				TinyGC::recycle(mGCInfo);
			}

			delete this;
		}
	}


	//-----------------------------------------------------------------
	//
	//	TinyBaseWeakRef
	//
	TinyBaseWeakRef::~TinyBaseWeakRef()
	{
		this->release();
		mGCInfo = nullptr;
	}

	void TinyBaseWeakRef::release()
	{
		if (--mGCInfo->weakRef < 1)
		{
			if (mGCInfo->strongRef == -623)
			{
				return;
			}

			if (mGCInfo->strongRef < 1)
			{
				TinyGC::recycle(mGCInfo);
			}
		}
	}
}

