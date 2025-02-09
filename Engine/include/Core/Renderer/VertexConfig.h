#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Head/RenderConfig.h"

namespace tezcat::Tiny
{
	enum TINY_API VertexPosition : uint32
	{
		VP_Position = 0,
		VP_Normal,
		VP_UV,
		VP_Color,
	};

	enum class TINY_API VertexLayoutType : uint32
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
		uint64 stride = 0;

		uint64 dataLength;

		const void* data;

		//标准化
		bool nomarlized = false;
	};


	class TINY_API VertexLayout
	{
	public:
		static int32 getTypeLength(VertexLayoutType type);
		static uint32 getTypeSize(VertexLayoutType type);
		static VertexLayoutType getLayoutType(VertexPosition position);
	};
}

