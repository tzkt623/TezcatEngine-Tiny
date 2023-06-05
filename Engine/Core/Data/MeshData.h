#pragma once

#include "../Head/CppHead.h"
#include "../Head/GLMHead.h"
#include "../Head/ConfigHead.h"
#include "../Renderer/VertexConfig.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	/// <summary>
	/// mesh数据
	/// 只有父节点会被保存到管理器中
	/// 子节点数据一般都是用来组成整个mesh
	/// 所以并不需要单独存储使用
	/// </summary>
	class TINY_API MeshData : public TinyObject
	{
		MeshData();
		MeshData(const std::string& name);
		TINY_Factory(MeshData);
		TINY_RTTI_H(MeshData);
	public:

		virtual ~MeshData();

		const std::string& getName() { return mName; }

		size_t vertexSize()
		{
			return this->mVertices.size() * sizeof(glm::vec3);
		}

		size_t normalSize()
		{
			return this->mNormals.size() * sizeof(glm::vec3);
		}

		size_t colorSize()
		{
			return this->mColors.size() * sizeof(glm::vec4);
		}

		size_t uvSize()
		{
			return this->mUVs.size() * sizeof(glm::vec2);
		}

		size_t indexSize()
		{
			return this->mIndices.size() * sizeof(unsigned int);
		}

		int getBufferSize();

		std::tuple<size_t, const void*> getVertexData(const VertexPosition& position);
		std::tuple<size_t, const void*> getIndexData();

		void apply(DrawMode drawMode = DrawMode::Triangles);

	public:
		bool hasChildren() { return mChildrenData != nullptr; }
		void addChild(MeshData* meshData);
		const std::vector<MeshData*>& getChildren() { return *mChildrenData; }


	public:
		int mIndex;
		std::string mName;
		std::vector<glm::vec3> mVertices;
		std::vector<glm::vec3> mNormals;
		std::vector<glm::vec4> mColors;
		std::vector<glm::vec2> mUVs;
		std::vector<glm::vec3> mTangents;
		std::vector<glm::vec3> mBitTangents;

		std::vector<unsigned int> mIndices;

		DrawMode mDrawMode;
		std::vector<VertexPosition> mLayoutPositions;

	private:
		std::vector<MeshData*>* mChildrenData;
	};


	class Texture;

	class ModelNode
	{
	public:
		ModelNode();
		~ModelNode();

		bool hasChildren() { return mChildrenData != nullptr; }
		bool hasMesh() { return mMeshData != nullptr; }
		void addChild(ModelNode* node);

		std::string mName;
		MeshData* mMeshData;
		std::vector<ModelNode*>* mChildrenData;
	};

	class Model : public ModelNode
	{
	public:
		Model();
		virtual ~Model();

	private:
		std::string mName;
	};


}

