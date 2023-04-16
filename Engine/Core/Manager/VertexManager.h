#pragma once

#include "Manager.h"

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

#include "Utility/Utility.h"

namespace tezcat::Tiny::Core
{
	class Vertex;
	class VertexBuffer;
	class MeshData;


	class TINY_API VertexCreator
	{
	public:
		virtual ~VertexCreator() = default;

		virtual VertexBuffer* createVertexBuffer() = 0;
		virtual Vertex* createVertex() = 0;

	};

	class TINY_API VertexManager : public Manager<VertexCreator>
	{
	public:
		VertexManager();
		virtual ~VertexManager();

	public:
		Vertex* createVertex(MeshData* meshData);

	public:
		void addVertexGroup(Vertex* vao);
		void addVertexGroup(MeshData* meshData);
		Vertex* getVertex(const std::string& name);

		void addMeshData(MeshData* meshData);
		MeshData* findMeshData(const std::string& name);

	private:
		VertexBuffer* createVertexBuffer(MeshData* meshData);
		void buildChild(Vertex* vertexParent, MeshData* meshParent);

	private:
		std::unordered_map<std::string, Vertex*> mVertexUMap;
		std::unordered_map<std::string, MeshData*> mMeshDataUMap;
	};



	using VertexMgr = SG<VertexManager>;
}
