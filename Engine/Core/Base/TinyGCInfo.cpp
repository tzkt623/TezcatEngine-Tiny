#include "TinyGCInfo.h"
#include "TinyGC.h"

namespace tezcat::Tiny
{
	//TinyGCInfo* TinyGCInfo::Default = new TinyGCInfo(0, -9999999, -9999999, nullptr);

	TinyGCInfo::TinyGCInfo(TinyGCInfoID index, int strongRef, int weakRef, TinyRefObject* pointer)
		: index(index)
		, strongRef(strongRef)
		, weakRef(weakRef)
		, pointer(pointer)
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
