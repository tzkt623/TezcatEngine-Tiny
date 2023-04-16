#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class MeshData;
	/// <summary>
	/// 对应gl的VBO
	/// </summary>
	class TINY_API VertexBuffer
	{
	public:
		VertexBuffer();
		virtual ~VertexBuffer() = 0;

		size_t getSize() const { return mSize; }
		virtual void init(MeshData* meshData) = 0;

	private:
		size_t mSize;
	};
}
