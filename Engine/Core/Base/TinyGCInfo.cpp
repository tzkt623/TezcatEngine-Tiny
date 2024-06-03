#include "TinyGCInfo.h"
#include "TinyGC.h"

namespace tezcat::Tiny
{
	//TinyGCInfo* TinyGCInfo::Default = new TinyGCInfo(0, -9999999, -9999999, nullptr);
	const int32 TinyGCInfo::EmptyStrongID = -623;

	TinyGCInfo::TinyGCInfo(TinyGCInfoID index, int32 strongRef, int32 weakRef, TinyRefObject* pointer)
		: index(index)
		, strongRef(strongRef)
		, weakRef(weakRef)
		, pointer(pointer)
		, unique(false)
	{

	}

// 	TinyGCInfo::TinyGCInfo(TinyGCInfo&& other) noexcept
// 	{
// 		this->pointer = other.pointer;
// 		this->strongRef = other.strongRef;
// 		this->weakRef = other.weakRef;
// 
// 		other.strongRef = -1;
// 		other.weakRef = -1;
// 		other.pointer = nullptr;
// 	}

	TinyGCInfo::~TinyGCInfo()
	{
		this->pointer = nullptr;
	}


}
