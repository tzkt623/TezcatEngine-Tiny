#pragma once

#include "Manager.h"

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

#include "../Tool/Tool.h"

namespace tezcat::Tiny
{
	class Vertex;
	class VertexBuffer;
	class IndexBuffer;
	class MeshData;


	class TINY_API VertexCreator
	{
	public:
		virtual ~VertexCreator() = default;

		virtual VertexBuffer* createVertexBuffer() = 0;
		virtual IndexBuffer* createIndexBuffer() = 0;
		virtual Vertex* createVertex() = 0;

	};

	class TINY_API VertexManager : public Manager<VertexCreator>
	{
	public:
		VertexManager();
		virtual ~VertexManager();

	public:
		Vertex* createVertex(MeshData* meshData);
		VertexBuffer* createVertexBuffer(const void* data, const size_t& length);
		VertexBuffer* createVertexBuffer(const size_t& length);
		IndexBuffer* createIndexBuffer(const void* data, const size_t& length);

	public:
		Vertex* getVertex(const std::string& name);
		Vertex* getVertex(MeshData* meshData);

		void addMeshData(MeshData* meshData);
		MeshData* findMeshData(const std::string& name);

	private:

		void buildVertex(Vertex* vertex, MeshData* meshData);
		void buildChild(Vertex* vertexParent, MeshData* meshParent);
		Vertex* findVertex(const std::string& name);

	private:
		std::unordered_map<std::string, Vertex*> mVertexUMap;
		std::unordered_map<std::string, MeshData*> mMeshDataUMap;
	};



	using VertexMgr = SG<VertexManager>;
}
