#include "Statistic.h"

namespace tezcat::Tiny
{
	int Statistic::PassCount = 0;
	int Statistic::DrawCall = 0;
	glm::vec2 Statistic::mouseOffset(0.0f);
	glm::vec2 Statistic::mousePosition;


	size_t Statistic::MemoryAlloc;
	size_t Statistic::MemoryFree;

	double Statistic::RenderTime;
	double Statistic::LogicTime;

	const unsigned char* Statistic::GPU;

	UniformF3* Statistic::MatPBR_Albedo = nullptr;
	UniformF1* Statistic::MatPBR_Metallic = nullptr;
	UniformF1* Statistic::MatPBR_Roughness = nullptr;
	UniformF1* Statistic::MatPBR_AO = nullptr;

}


