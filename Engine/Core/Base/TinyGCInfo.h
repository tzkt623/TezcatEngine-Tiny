#pragma once

#include "../Head/ConfigHead.h"
#include "../Head/CppHead.h"

namespace tezcat::Tiny
{
	using TinyGCInfoID = int;

	class TinyRefObject;
	struct TINY_API TinyGCInfo
	{
		TinyGCInfoID index = 0;
		int strongRef = 0;
		int weakRef = 0;
		TinyRefObject* pointer = nullptr;

		TinyGCInfo() {}
		TinyGCInfo(TinyGCInfoID index, int strongRef, int weakRef, TinyRefObject* pointer);
		//TinyGCInfo(TinyGCInfo&& other) noexcept;

		~TinyGCInfo();


		//static TinyGCInfo* Default;
	};
}
