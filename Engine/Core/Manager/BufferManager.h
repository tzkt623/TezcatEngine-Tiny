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


	class TINY_API BufferCreator
	{
	public:
		virtual ~BufferCreator() = default;

		virtual VertexBuffer* createVertexBuffer() = 0;
		virtual IndexBuffer* createIndexBuffer() = 0;
		virtual Vertex* createVertex() = 0;

	};

	class TINY_API BufferManager : public Manager<BufferCreator>
	{
	public:
		BufferManager();
		virtual ~BufferManager();

	public:
		/*
		* @author HCL
		* @info 2023|5|16
		* @brief 创建Vertex并且保存
		*/
		Vertex* createVertexAndCached(MeshData* meshData);


		/*
		* @author HCL
		* @info 2023|5|16
		* @brief 创建Vertex但是不保存
		*/
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

	using BufferMgr = SG<BufferManager>;



}
