#pragma once

#include "../Head/CppHead.h"
#include "../Head/GLMHead.h"
#include "../Head/ConfigHead.h"
#include "../Renderer/VertexConfig.h"

namespace tezcat::Tiny
{
	/// <summary>
	/// mesh数据
	/// 只有父节点会被保存到管理器中
	/// 子节点数据一般都是用来组成整个mesh
	/// 所以并不需要单独存储使用
	/// </summary>
	class TINY_API MeshData
	{
	public:
		MeshData();
		MeshData(const std::string& name);
		MeshData(MeshData&& other) noexcept;
		~MeshData();

		const std::string& getName() { return name; }

		size_t vertexSize()
		{
			return this->vertices.size() * sizeof(glm::vec3);
		}

		size_t normalSize()
		{
			return this->normals.size() * sizeof(glm::vec3);
		}

		size_t colorSize()
		{
			return this->colors.size() * sizeof(glm::vec4);
		}

		size_t uvSize()
		{
			return this->uvs.size() * sizeof(glm::vec2);
		}

		size_t indexSize()
		{
			return this->indices.size() * sizeof(unsigned int);
		}

		int getBufferSize();

		const void* getVertexData(const VertexPosition& position, size_t& outDataLength);
		const void* getIndexData(size_t& outDataLength);

		void apply(DrawMode drawMode = DrawMode::Triangles);

	public:
		bool hasChildren() { return mChildrenData != nullptr; }
		void addChild(MeshData* meshData);
		const std::vector<MeshData*>& getChildren() { return *mChildrenData; }

	public:
		MeshData& operator=(MeshData&& other) noexcept;

	public:
		std::string name;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec4> colors;
		std::vector<glm::vec2> uvs;

		std::vector<unsigned int> indices;
		DrawMode drawMode;

		std::vector<VertexPosition> layoutPositions;


	private:
		std::vector<MeshData*>* mChildrenData;
	};
}

