#include "Statistic.h"

namespace tezcat::Tiny
{
	int Statistic::PassCount = 0;
	int Statistic::DrawCall = 0;
	glm::vec2 Statistic::mouseOffset(0.0f);
	glm::vec2 Statistic::mousePosition;


	size_t Statistic::MemoryAlloc;
	size_t Statistic::MemoryFree;

}


