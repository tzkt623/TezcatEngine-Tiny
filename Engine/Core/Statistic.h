#pragma once
#include "Head/GLMHead.h"
#include "Head/ConfigHead.h"
#include "Core/Shader/Uniform.h"

namespace tezcat::Tiny
{
	struct TINY_API Statistic
	{
		static const unsigned char* GPU;

		static int PassCount;
		static int DrawCall;
		static glm::vec2 mouseOffset;
		static glm::vec2 mousePosition;

		static double RenderTime;
		static double LogicTime;

		static size_t MemoryAlloc;
		static size_t MemoryFree;

		static size_t getMemoryUse() { return MemoryAlloc - MemoryFree; }


		static UniformF3* MatPBR_Albedo;
		static UniformF1* MatPBR_Roughness;
		static UniformF1* MatPBR_Metallic;
		static UniformF1* MatPBR_AO;
	};
}
