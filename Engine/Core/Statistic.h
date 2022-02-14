#pragma once
#include "Head/GLMHead.h"

namespace tezcat::Tiny
{
	struct Statistic
	{
		static int PassCount;
		static int DrawCall;
		static glm::vec2 mouseOffset;
		static glm::vec2 mousePosition;

		static size_t MemoryAlloc;
		static size_t MemoryFree;

		static size_t getMemoryUse() { return MemoryAlloc - MemoryFree; }
	};
}
