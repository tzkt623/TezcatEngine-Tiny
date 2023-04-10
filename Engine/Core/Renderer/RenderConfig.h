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

	enum class ClearOption : uint32_t
	{
		None = 0,
		Skybox = 1 << 0,
		Depth = 1 << 1,
		Color = 1 << 2,
		Stencil = 1 << 3
	};

	// 	class TINY_API ClearOption
	// 	{
	//		ClearOption() = delete;
	//		~ClearOption() = delete;
	//	public:
	//		static const uint32_t None = 0;
	// 		static const uint32_t Skybox = 1 << 0;
	//		static const uint32_t Depth = 1 << 1;
	//		static const uint32_t ColorComponent = 1 << 2;
	//		static const uint32_t Stencil = 1 << 3;
	// 	}
}
