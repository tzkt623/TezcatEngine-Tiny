#include "TinyRefObject.h"
#include "TinyGC.h"

namespace tezcat::Tiny
{
	//-----------------------------------------------------------------
	//
	//	TinyRefObject
	//
	uint32 TinyRefObject::__ClassID = 1;
	const std::string TinyRefObject::Empty = "TinyRefObject";
	const TinyRTTI TinyRefObject::__TINY__RTTI__453{ nullptr, "TinyRefObject", typeid(TinyRefObject), 0 };

	TinyRefObject::TinyRefObject()
		: mGCInfo(nullptr)
	{
	}

	TinyRefObject::~TinyRefObject()
	{
		mGCInfo = nullptr;
	}

	void TinyRefObject::deleteObject()
	{
		if (--(mGCInfo->strongRef) < 1)
		{
			if (mGCInfo->weakRef < 1)
			{
				TinyGC::recycle(mGCInfo);
			}

			this->onClose();
			delete this;
		}
	}

	void TinyRefObject::autoGC()
	{
		mGCInfo = TinyGC::getNextGCInfo(this);
		TinyGC::manage(this);
	}
}

