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
		int32 getVertexCount() const { return mVertexCount; }
		int32 getIndexCount() const { return mIndexCount; }
		DrawModeWrapper& getDrawMode() { return mDrawModeWrapper; }

		uint32 getVertexID() const { return mVertexID; }
		void apply(uint32 id) { mVertexID = id; }
		void generate();

		void setVertexBuffer(VertexBuffer* buffer);
		void setIndexBuffer(IndexBuffer* buffer);
		void setVertexCount(uint32 size) { mVertexCount = size; }
		void setIndexCount(uint32 size) { mIndexCount = size; }

		void addChild(Vertex* vertex);

		const std::vector<VertexBuffer*>& getVertexBuffers() const
		{
			return mVertexBuffers;
		}

		IndexBuffer* getIndexBuffer() const
		{
			return mIndexBuffer;
		}

	public:
		void setMesh(MeshData* mesh);
		void setMesh(const std::string& name, const uint32& vertexCount, const DrawMode& drawMode);

	protected:
		virtual void onClose() override;

	protected:
		std::string mName;
		uint32 mVertexID = 0;

		uint32 mVertexCount = 0;
		uint32 mIndexCount = 0;

		DrawModeWrapper mDrawModeWrapper;
		std::vector<VertexBuffer*> mVertexBuffers;
		IndexBuffer* mIndexBuffer;
		std::vector<Vertex*>* mChildren;
	};
}
