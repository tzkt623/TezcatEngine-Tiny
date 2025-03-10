﻿#pragma once
/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


#include "../Head/CppHead.h"
#include "../Head/GLMHead.h"
#include "../Head/ConfigHead.h"
#include "../Renderer/VertexConfig.h"
#include "../Base/TinyObject.h"

struct aiMesh;
struct aiNode;
struct aiScene;
struct aiMaterial;

namespace tezcat::Tiny
{

	class GameObject;
	/// <summary>
	/// mesh数据
	/// 只有父节点会被保存到管理器中
	/// 子节点数据一般都是用来组成整个mesh
	/// 所以并不需要单独存储使用
	/// </summary>
	class TINY_API MeshData : public TinyObject
	{
		MeshData();
		MeshData(std::string name);
		TINY_OBJECT_H(MeshData, TinyObject)
	public:

		virtual ~MeshData();

		const std::string& getName() const { return mName; }

		uint64 vertexSize() const
		{
			return this->mVertices.size() * sizeof(float3);
		}

		uint64 normalSize() const
		{
			return this->mNormals.size() * sizeof(float3);
		}

		uint64 colorSize() const
		{
			return this->mColors.size() * sizeof(float4);
		}

		uint64 uvSize() const
		{
			return this->mUVs.size() * sizeof(float2);
		}

		uint64 indexSize() const
		{
			return this->mIndices.size() * sizeof(unsigned int);
		}

		int getBufferSize() const;

		std::tuple<uint64, const void*> getVertexData(const VertexPosition& position);
		std::tuple<uint64, const void*> getIndexData();

		void apply(DrawMode drawMode = DrawMode::Triangles);

	public:
		bool hasChildren() { return mChildrenData != nullptr; }
		void addChild(MeshData* meshData);
		const std::vector<MeshData*>& getChildren() { return *mChildrenData; }


	public:
		int mIndex;
		std::string mName;
		std::vector<float3> mVertices;
		std::vector<float3> mNormals;
		std::vector<float4> mColors;
		std::vector<float2> mUVs;
		std::vector<float3> mTangents;
		std::vector<float3> mBitTangents;

		std::vector<uint32> mIndices;

		DrawMode mDrawMode;
		std::vector<VertexPosition> mLayoutPositions;

	private:
		std::vector<MeshData*>* mChildrenData;
	};


	class Texture;
	class Vertex;
	class ModelNode
	{
	public:
		ModelNode();
		~ModelNode();

		bool hasChildren() const { return mChildrenCount != 0; }
		bool hasMesh() const { return mVertex != nullptr; }

		void init(uint32 meshCount, uint32 childCount);
		void addChild(ModelNode* node);
		void setVertex(Vertex* vertex);


		std::string mName;
		Vertex* mVertex;

		bool mInited;
		uint32 mIndex;
		ModelNode** mChildren;
		uint32 mChildrenCount;
		int32_t mParentIndex;
	};

	class Model : public TinyObject
	{
		Model();
		TINY_OBJECT_H(Model, TinyObject)
	public:
		virtual ~Model();

		void load(const std::string& path);
		GameObject* generate();

		void setPath(const std::string& path) { mPath = path; }
		std::string_view getPath() { return mPath; }

	private:
		void createModelNode(const aiScene* aiscene, aiNode* ainode, int32_t parentIndex);
		MeshData* createMesh(aiMesh* aimesh, const aiScene* aiscene, const aiNode* node);
		void setMaterial(ModelNode* child, aiMaterial* aiMaterial);

	private:
		std::string mName;
		std::string_view mPath;

		std::vector<ModelNode*> mChildren;
		

		//ModelNode* mRoot;
		//std::vector<ModelNode> mChildren;
	};
}

