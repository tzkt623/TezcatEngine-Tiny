#include "CppHead.h"
#include "../Profiler.h"


#ifndef TINY_Release
using namespace tezcat::Tiny;

void* operator new(std::size_t size)
{
	Profiler::MemoryAlloc += size;
	return malloc(size);
}

void operator delete(void* memory, std::size_t size)
{
	Profiler::MemoryAlloc -= size;
	free(memory);
}

void* operator new[](std::size_t size)
{
	Profiler::MemoryAlloc += size;
	return malloc(size);
}

void operator delete[](void* memory, std::size_t size)
{
	Profiler::MemoryAlloc -= size;
	free(memory);
}
#endif // !TINY_Release
