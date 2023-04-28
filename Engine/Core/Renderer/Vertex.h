#pragma once

#include "../Head/TinyCpp.h"
#include "../Head/ConfigHead.h"
#include "../Head/Context.h"
#include "VertexConfig.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	class MeshData;
	class VertexBuffer;
	class IndexBuffer;

	/// <summary>
	/// 对应gl的VAO
	/// 
	/// </summary>
	class TINY_API Vertex : public TinyObject
	{
		TINY_RTTI_H(Vertex)

	public:
		Vertex();
		virtual ~Vertex();
		void setName(const std::string& val) { mName = val; }
		std::string getName() const { return mName; }
		int getVertexCount() const { return mVertexCount; }
		int getIndexCount() const { return mIndexCount; }
		DrawModeWrapper& getDrawMode() { return mDrawModeWrapper; }

		uint32_t getUID() const { return mUID; }
		VertexLayout& getLayout() { return mLayout; }

		virtual void setVertexBuffer(VertexBuffer* buffer);
		virtual void setIndexBuffer(IndexBuffer* buffer);
		void setVertexCount(size_t size) { mVertexCount = size; }
		void setIndexCount(size_t size) { mIndexCount = size; }

		void addChild(Vertex* vertex);

	public:
		virtual void init(MeshData* mesh);
		virtual void init(const std::string& name, const size_t& vertexCount, const DrawMode& drawMode);
		virtual void bind() = 0;
		virtual void unbind() = 0;

	protected:
		std::string mName;
		uint32_t mUID = 0;

		uint32_t mVertexCount = 0;
		uint32_t mIndexCount = 0;

		VertexLayout mLayout;
		DrawModeWrapper mDrawModeWrapper;
		TinyVector<VertexBuffer*> mVertexBuffers;
		IndexBuffer* mIndexBuffer = nullptr;
		TinyVector<Vertex*>* mChildren = nullptr;
	};
}
