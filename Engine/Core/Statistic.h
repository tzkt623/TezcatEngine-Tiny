#pragma once
#include "glm/glm.hpp"


namespace tezcat::Tiny
{
	struct Statistic
	{
		static int PassCount;
		static int DrawCall;
		static glm::vec2 mouseOffset;
		static glm::vec2 mousePosition;
	};
}
