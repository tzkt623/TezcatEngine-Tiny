#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class MeshData;
	class TINY_API VertexBuffer
	{
	public:
		VertexBuffer();
		virtual ~VertexBuffer() = 0;

		size_t getSize() const { return m_Size; }
		void* getData() const { return m_Data; }

	protected:
		virtual void init(MeshData* meshData) = 0;

	private:
		size_t m_Size;
		void* m_Data;
	};
}