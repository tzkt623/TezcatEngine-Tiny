#pragma once

#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	struct TINY_API ViewportInfo
	{
		int OX;
		int OY;
		int Width;
		int Height;

		ViewportInfo()
			: OX(0)
			, OY(0)
			, Width(0)
			, Height(0)
		{

		}

		ViewportInfo(const int& ox, const int& oy, const int& width, const int& height)
			: OX(ox)
			, OY(oy)
			, Width(width)
			, Height(height)
		{

		}
	};

	enum ClearOption : uint32_t
	{
		CO_None = 0,
		CO_Skybox = 1 << 0,
		CO_Depth = 1 << 1,
		CO_Color = 1 << 2,
		CO_Stencil = 1 << 3
	};

	enum class TINY_API RenderPassType
	{
		Default,
		Shadow,
		Addition
	};
}
