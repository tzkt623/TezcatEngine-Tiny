#pragma once
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
#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Renderer/RenderConfig.h"

namespace tezcat::Tiny
{
	enum TINY_API VertexPosition : uint32_t
	{
		VP_Position = 0,
		VP_Normal,
		VP_UV,
		VP_Tangent,
		VP_BiTangent,
		VP_Color,
		VP_ModuleMatrix
	};

	enum class TINY_API VertexLayoutType : uint32_t
	{
		None = 0,
		Float1,
		Float2,
		Float3,
		Float4,
		Mat3x3,
		Mat4x4
	};

	struct TINY_API VertexLayoutData
	{
		//顶点的位置
		VertexPosition position;
		//顶点的数据类型
		VertexLayoutType type;
		//结构大小
		uint64_t stride = 0;

		uint64_t dataLength;

		const void* data;

		//标准化
		bool nomarlized = false;
	};


	class TINY_API VertexLayout
	{
	public:
		static int32_t getTypeLength(VertexLayoutType type);
		static uint32_t getTypeSize(VertexLayoutType type);
		static VertexLayoutType getLayoutType(VertexPosition position);
	};
}

