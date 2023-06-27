#pragma once

#include "Manager.h"

#include "../Head/CppHead.h"
#include "../Tool/Tool.h"

namespace tezcat::Tiny
{
	class Vertex;
	class VertexBuffer;
	class IndexBuffer;
	class MeshData;

	class TINY_API VertexBufferManager
	{
	public:
		VertexBufferManager();
		virtual ~VertexBufferManager();


	public:
		Vertex* create(const std::string& name);
		Vertex* create(MeshData* mesh);

		void add(MeshData* meshData);
		void add(Vertex* vertex);

		MeshData* findMeshData(const std::string& name);
		Vertex* findVertex(const std::string& name);

	private:
		std::unordered_map<std::string, Vertex*> mVertexUMap;
		std::unordered_map<std::string, MeshData*> mMeshDataUMap;
		std::vector<Vertex*> mVertexAry;
	};

	using VertexBufMgr = SG<VertexBufferManager>;
}
