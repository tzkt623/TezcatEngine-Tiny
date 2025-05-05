#pragma once
/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

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

		uint64_t vertexSize() const
		{
			return mVertices.size() * sizeof(float3);
		}

		uint64_t normalSize() const
		{
			return mNormals->size() * sizeof(float3);
		}

		uint64_t tangentSize() const
		{
			return mTangents->size() * sizeof(float3);
		}

		uint64_t bitangentSize() const
		{
			return mBiTangents->size() * sizeof(float3);
		}

		uint64_t colorSize() const
		{
			return mColors->size() * sizeof(float4);
		}

		uint64_t uvSize() const
		{
			return mUVs->size() * sizeof(float2);
		}

		uint64_t indexSize() const
		{
			return mIndices->size() * sizeof(uint32_t);
		}

		std::tuple<uint64_t, const void*> getVertexData(const VertexPosition& position);
		std::tuple<uint64_t, const void*> getIndexData();

		void generate(DrawMode drawMode = DrawMode::Triangles);

		bool generateTangents();

		void createNormals()
		{
			if (mNormals)
			{
				return;
			}
			mNormals = new std::vector<float3>();
		}
		void createColors()
		{
			if (mColors)
			{
				return;
			}
			mColors = new std::vector<float4>();
		}
		void createUVs()
		{
			if (mUVs)
			{
				return;
			}
			mUVs = new std::vector<float2>();
		}
		void createTangents()
		{
			if (mTangents)
			{
				return;
			}
			mTangents = new std::vector<float3>();
			mBiTangents = new std::vector<float3>();
		}

		void createIndices()
		{
			if (mIndices)
			{
				return;
			}
			mIndices = new std::vector<uint32_t>();
		}

	public:
		std::string mName;
		int mIndex;
		DrawMode mDrawMode;
		std::vector<VertexPosition> mLayoutPositions;

		std::vector<float3> mVertices;
		std::vector<float3>* mNormals;
		std::vector<float4>* mColors;
		std::vector<float2>* mUVs;
		std::vector<float3>* mTangents;
		std::vector<float3>* mBiTangents;

		std::vector<uint32_t>* mIndices;
	};


	class Texture;
	class Vertex;
	class Shader;
	class ModelNode
	{
	public:
		union Value
		{
			struct Color
			{
				float r;
				float g;
				float b;
				float a;
			} color;

			float v1;
			float v2;
			float v3;
			float v4;
		};

		ModelNode();
		~ModelNode();

		void setVertex(Vertex* vertex);

		std::tuple<bool, std::string_view> getTextureMap(MaterialTextureSlot slot);
		std::tuple<bool, Value> getValueMap(MaterialTextureSlot slot);


		std::unordered_map<MaterialTextureSlot, std::string>& getTextureMap();
		std::unordered_map<MaterialTextureSlot, Value>& getValueMap();

		std::string* tryGetTexture(MaterialTextureSlot slot);
		Value* tryGetValue(MaterialTextureSlot slot);

		void setBlend(Blend source, Blend target);

	public:
		Vertex* mVertex;
		std::string mName;
		uint32_t mIndex;
		int32_t mParentIndex;
		Shader* mShader;

		bool mIsEnableBlend;
		bool mIsPBR;
		bool mHasTexture;
		Blend mSource;
		Blend mTarget;


	private:
		std::unordered_map<MaterialTextureSlot, std::string>* mTextureMap;
		std::unordered_map<MaterialTextureSlot, Value> *mValueMap;
	};

	class Model : public TinyObject
	{
		Model();
		TINY_OBJECT_H(Model, TinyObject)
	public:
		virtual ~Model();

		bool load(const file_path& path);
		GameObject* generate();

		void setPath(const file_path& path) { mPath = path; }
		const file_path& getPath() { return mPath; }

	private:
		void setShading(ModelNode* node, aiMaterial* aiMaterial);
		void setBlend(ModelNode* node, aiMaterial* aiMaterial);
	private:
		void createModelNode(const aiScene* aiscene, aiNode* ainode, int32_t parentIndex);
		MeshData* createMesh(aiMesh* aimesh, const aiScene* aiscene, const aiNode* node);
		void setMaterial(ModelNode* child, aiMaterial* aiMaterial);

	private:
		std::string mName;
		file_path mPath;
		std::vector<ModelNode*> mChildren;
	};

	class AssimpHelper
	{
	public:
		static bool isPBRValue(aiMaterial* material);
		static bool isPBRTexture(aiMaterial* material);
	};
}

