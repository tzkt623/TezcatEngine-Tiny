#include "VertexConfig.h"

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
