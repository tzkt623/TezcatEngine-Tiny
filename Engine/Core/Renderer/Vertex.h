#pragma once
#include "VertexConfig.h"

#include "../Head/TinyCpp.h"
#include "../Head/ConfigHead.h"
#include "../Head/RenderConfig.h"
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
		Vertex();
		Vertex(std::string name);
		TINY_OBJECT_H(Vertex, TinyObject)

	protected:
		virtual void init() override {}

	public:
		virtual ~Vertex();
		void setName(const std::string& val) { mName = val; }
		std::string getName() const { return mName; }
		int getVertexCount() const { return mVertexCount; }
		int getIndexCount() const { return mIndexCount; }
		DrawModeWrapper& getDrawMode() { return mDrawModeWrapper; }

		uint32 getVertexID() const { return mVertexID; }
		void apply(uint32 id) { mVertexID = id; }
		void generate();

		void setVertexBuffer(VertexBuffer* buffer);
		void setIndexBuffer(IndexBuffer* buffer);
		void setVertexCount(size_t size) { mVertexCount = (uint32)size; }
		void setIndexCount(size_t size) { mIndexCount = (uint32)size; }

		void addChild(Vertex* vertex);

		TinyVector<VertexBuffer*>& getVertexBuffers()
		{
			return mVertexBuffers;
		}

		IndexBuffer* getIndexBuffer()
		{
			return mIndexBuffer;
		}

	public:
		void init(MeshData* mesh);
		void init(const std::string& name, const size_t& vertexCount, const DrawMode& drawMode);

	protected:
		std::string mName;
		uint32 mVertexID = 0;

		uint32 mVertexCount = 0;
		uint32 mIndexCount = 0;

		DrawModeWrapper mDrawModeWrapper;
		TinyVector<VertexBuffer*> mVertexBuffers;
		IndexBuffer* mIndexBuffer = nullptr;
		TinyVector<Vertex*>* mChildren = nullptr;
	};
}
