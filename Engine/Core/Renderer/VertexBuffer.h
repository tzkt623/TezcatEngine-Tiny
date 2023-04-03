#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Tool/DelegateCreator.h"

namespace tezcat::Tiny::Core
{
	class MeshData;
	class TINY_API VertexBuffer
	{
	public:
		VertexBuffer();
		virtual ~VertexBuffer() = 0;

		size_t getSize() const { return mSize; }
		void* getData() const { return mData; }

	protected:
		virtual void init(MeshData* meshData) = 0;

	private:
		size_t mSize;
		void* mData;
	};

	class VertexBufferCreatorImp
	{
	public:
		virtual ~VertexBufferCreatorImp() = default;
		virtual VertexBuffer* create(MeshData* meshData) = 0;
	};

	using VertexBufferCreator = DelegateCreator<VertexBufferCreatorImp, VertexBuffer>;

}
