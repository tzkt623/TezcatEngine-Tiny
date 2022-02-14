#include "CppHead.h"
#include "../Statistic.h"

#define MemoryCheck

#ifdef MemoryCheck

#if false
#define Check() std::cout << Statistic::getMemoryUse() / 1024.0 << " kb" << std::endl;
#else
#define Check()
#endif

using namespace tezcat::Tiny;

void* operator new(size_t size)
{
	Statistic::MemoryAlloc += size;
	Check()
	return malloc(size);
}

void operator delete(void* memory, size_t size)
{
	Statistic::MemoryFree += size;
	Check()
	free(memory);
}

void* operator new[](size_t size)
{
	Statistic::MemoryAlloc += size;
	Check()
	return malloc(size);
}

void operator delete[](void* memory, size_t size)
{
	Statistic::MemoryFree += size;
	Check()
	free(memory);
}
#endif // MemoryCheck