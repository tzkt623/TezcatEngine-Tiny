#pragma once

#include "../Head/CppHead.h"
#include "../Tool/Tool.h"

namespace tezcat::Tiny
{
	class Vertex;
	class VertexBuffer;
	class IndexBuffer;
	class MeshData;
	class UniformBuffer;
	class UniformBufferLayout;

	class TINY_API VertexBufferManager
	{
		VertexBufferManager() = delete;
		~VertexBufferManager() = delete;

	public:
		static Vertex* create(const std::string& name);
		static Vertex* create(MeshData* mesh);

		static void add(MeshData* meshData);
		static void add(Vertex* vertex);

		static MeshData* findMeshData(const std::string& name);
		static Vertex* findVertex(const std::string& name);

		static void init();

	private:
		static std::unordered_map<std::string_view, Vertex*> mVertexUMap;
		static std::unordered_map<std::string_view, MeshData*> mMeshDataUMap;
		static std::vector<Vertex*> mVertexAry;


	public:
		static std::tuple<bool, std::shared_ptr<UniformBufferLayout>> createUniformBufferLayout(const std::string& name);
		static std::shared_ptr<UniformBufferLayout> getUniformBufferLayout(const std::string& name);

	private:
		static std::unordered_map<std::string, std::shared_ptr<UniformBufferLayout>> mUniformBufferMap;
	};
}
