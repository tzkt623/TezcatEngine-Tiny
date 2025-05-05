#pragma once

/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "../Renderer/RenderConfig.h"

namespace tezcat::Tiny
{
	enum class TINY_API RangeType : int8_t
	{
		Null = -1,
		Color,
		Number,
		Count
	};

	struct TINY_API BaseRange
	{
		enum class Type
		{
			Float,
			Int,
			UInt,
			Double,
		};

		virtual Type getType() = 0;
	};

	template<typename T>
	struct TINY_API RangeT : public BaseRange
	{
		T min;
		T max;

		RangeT(T min, T max)
			: min(min)
			, max(max)
		{}

		virtual ~RangeT() {}
	};

	struct TINY_API RangeFloat : public RangeT<float>
	{
		using RangeT::RangeT;
		Type getType() final { return BaseRange::Type::Float; }
	};

	struct TINY_API RangeInt : public RangeT<int32_t>
	{
		using RangeT::RangeT;
		Type getType() final { return BaseRange::Type::Int; }
	};

	struct TINY_API RangeUInt : public RangeT<uint32_t>
	{
		using RangeT::RangeT;
		Type getType() final { return BaseRange::Type::UInt; }
	};

	struct TINY_API GlobalSlotConfig
	{

	};
}
