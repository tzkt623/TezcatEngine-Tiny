#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Head/RenderConfig.h"

namespace tezcat::Tiny
{
	enum TINY_API VertexPosition : uint32_t
	{
		VP_Position = 0,
		VP_Normal,
		VP_UV,
		VP_Color,
	};

	enum class TINY_API VertexLayoutType : uint32_t
	{
		None = 0,
		Float1,
		Float2,
		Float3,
		Float4,
	};

	struct TINY_API VertexLayoutData
	{
		//顶点的位置
		VertexPosition position;
		//顶点的数据类型
		VertexLayoutType type;
		//结构大小
		size_t stride = 0;

		size_t dataLength;

		const void* data;

		//标准化
		bool nomarlized = false;
	};


	class TINY_API VertexLayout
	{
	public:
		static int getTypeLength(VertexLayoutType type);
		static uint32_t getTypeSize(VertexLayoutType type);
		static VertexLayoutType getLayoutType(VertexPosition position);
	};
}

