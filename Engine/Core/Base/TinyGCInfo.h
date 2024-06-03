#pragma once

#include "../Head/ConfigHead.h"
#include "../Head/CppHead.h"

namespace tezcat::Tiny
{
	using TinyGCInfoID = int32;

	class TinyRefObject;
	struct TINY_API TinyGCInfo
	{
		TinyGCInfoID index = 0;
		int32 strongRef = 0;
		int32 weakRef = 0;
		bool unique = false;
		TinyRefObject* pointer = nullptr;

		TinyGCInfo() {}
		TinyGCInfo(TinyGCInfoID index, int32 strongRef, int32 weakRef, TinyRefObject* pointer);
		//TinyGCInfo(TinyGCInfo&& other) noexcept;

		~TinyGCInfo();


		//static TinyGCInfo* Default;

		const static int32 EmptyStrongID;
	};
}
