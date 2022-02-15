#include "CppHead.h"
#include "../Statistic.h"

#define MemoryCheck 1

#if MemoryCheck

#define EnableLog 0

#if EnableLog
#define Log() std::cout << Statistic::getMemoryUse() / 1024.0 << " kb" << std::endl
#else
#define Log()
#endif

using namespace tezcat::Tiny;

void* operator new(std::size_t size)
{
	Statistic::MemoryAlloc += size;
	Log();
	return malloc(size);
}

void operator delete(void* memory, std::size_t size)
{
	Statistic::MemoryFree += size;
	Log();
	free(memory);
}

void* operator new[](std::size_t size)
{
	Statistic::MemoryAlloc += size;
	Log();
	return malloc(size);
}

void operator delete[](void* memory, std::size_t size)
{
	Statistic::MemoryFree += size;
	Log();
	free(memory);
}
#endif // MemoryCheck