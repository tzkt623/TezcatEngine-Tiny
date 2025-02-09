/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

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

#include "Core/Renderer/VertexConfig.h"

namespace tezcat::Tiny
{

	int32 VertexLayout::getTypeLength(VertexLayoutType type)
	{
		switch (type)
		{
		case VertexLayoutType::Float1: return 1;
		case VertexLayoutType::Float2: return 2;
		case VertexLayoutType::Float3: return 3;
		case VertexLayoutType::Float4: return 4;
		default:
			break;
		}

		return 0;
	}

	uint32 VertexLayout::getTypeSize(VertexLayoutType type)
	{
		switch (type)
		{
		case VertexLayoutType::Float1: return sizeof(float);
		case VertexLayoutType::Float2: return sizeof(float) * 2;
		case VertexLayoutType::Float3: return sizeof(float) * 3;
		case VertexLayoutType::Float4: return sizeof(float) * 4;
		default:
			break;
		}

		return 0;
	}

	VertexLayoutType VertexLayout::getLayoutType(VertexPosition position)
	{
		switch (position)
		{
		case VertexPosition::VP_Position:
		case VertexPosition::VP_Normal:
			return VertexLayoutType::Float3;
		case VertexPosition::VP_UV:
			return VertexLayoutType::Float2;
		case VertexPosition::VP_Color:
			return VertexLayoutType::Float4;
		default:
			return VertexLayoutType::None;
		}
	}
}
