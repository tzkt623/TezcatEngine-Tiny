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

#include "Core/Data/MeshData.h"
#include "Core/Renderer/Vertex.h"
#include "Core/Component/Transform.h"
#include "Core/Component/GameObject.h"
#include "Core/Component/MeshRenderer.h"
#include "Core/Manager/ShaderManager.h"
#include "Core/Shader/Shader.h"

#include "ThirdParty/assimp/Importer.hpp"
#include "ThirdParty/assimp/scene.h"
#include "ThirdParty/assimp/postprocess.h"
#include "ThirdParty/Hash/city.h"

namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(MeshData, TinyObject)
	MeshData::MeshData()
		: MeshData("")
	{

	}

	MeshData::MeshData(std::string name)
		: mName(std::move(name))
		, mDrawMode(DrawMode::Triangles)
		, mChildrenData(nullptr)
		, mIndex(-1)
	{

	}

	MeshData::~MeshData()
	{
		this->mVertices.clear();
		this->mNormals.clear();
		this->mColors.clear();
		this->mUVs.clear();
		this->mIndices.clear();

		if (mChildrenData != nullptr)
		{
			for (auto c : *mChildrenData)
			{
				delete c;
			}

			delete mChildrenData;
		}
	}

	int MeshData::getBufferSize() const
	{
		int count = 0;
		if (!this->mVertices.empty())
		{
			count++;
		}

		if (!this->mNormals.empty())
		{
			count++;
		}

		if (!this->mColors.empty())
		{
			count++;
		}

		if (!this->mUVs.empty())
		{
			count++;
		}

		if (!this->mIndices.empty())
		{
			count++;
		}

		return count;
	}

	void MeshData::addChild(MeshData* meshData)
	{
		if (mChildrenData == nullptr)
		{
			mChildrenData = new std::vector<MeshData*>();
		}

		mChildrenData->push_back(meshData);
	}

	std::tuple<uint64, const void*> MeshData::getVertexData(const VertexPosition& position)
	{
		switch (position)
		{
		case VertexPosition::VP_Position:
			return { this->vertexSize(), this->mVertices.data() };
		case VertexPosition::VP_Normal:
			return { this->normalSize(), this->mNormals.data() };
		case VertexPosition::VP_Color:
			return  { this->colorSize(), this->mColors.data() };
		case VertexPosition::VP_UV:
			return  { this->uvSize(), this->mUVs.data() };
		default: return { 0, nullptr };
		}
	}

	std::tuple<uint64, const void*> MeshData::getIndexData()
	{
		return { this->indexSize(), this->mIndices.data() };
	}

	void MeshData::apply(DrawMode drawMode)
	{
		drawMode = drawMode;

		mLayoutPositions.emplace_back(VertexPosition::VP_Position);

		if (!this->mNormals.empty())
		{
			mLayoutPositions.emplace_back(VertexPosition::VP_Normal);
		}

		if (!this->mUVs.empty())
		{
			mLayoutPositions.emplace_back(VertexPosition::VP_UV);
		}

		if (!this->mColors.empty())
		{
			mLayoutPositions.emplace_back(VertexPosition::VP_Color);
		}
	}

	//--------------------------------------------------
	//
	//	ModelNode
	//
	ModelNode::ModelNode()
		: mChildren(nullptr)
		, mChildrenCount(0)
		, mVertex(nullptr)
		, mIndex(0)
		, mInited(false)
	{

	}

	ModelNode::~ModelNode()
	{
		if (mVertex)
		{
			mVertex->deleteObject();
		}

		if (mChildren)
		{
			for (uint32 i = 0; i < mChildrenCount; i++)
			{
				delete mChildren[i];
			}
		}
		delete[] mChildren;
	}

	void ModelNode::addChild(ModelNode* node)
	{
		mChildren[mIndex++] = node;
	}

	void ModelNode::setVertex(Vertex* vertex)
	{
		mVertex = vertex;
		mVertex->saveObject();
	}

	void ModelNode::init(uint32 meshCount, uint32 childCount)
	{
		if (mInited)
		{
			return;
		}

		mInited = true;
		if (meshCount > 1)
		{
			childCount += meshCount;
		}

		if (childCount > 0)
		{
			mChildrenCount = childCount;
			mChildren = new ModelNode * [mChildrenCount];
		}
	}


	TINY_OBJECT_CPP(Model, TinyObject)
	Model::Model()
		: mName()
		, mRoot(nullptr)
	{

	}

	Model::~Model()
	{
		delete mRoot;
	}

	void Model::foreachNode(const std::function<Transform* (ModelNode*)>& func)
	{
		auto transform = func(mRoot);
		if (mRoot->mChildrenCount > 0)
		{
			for (uint32 i = 0; i < mRoot->mChildrenCount; i++)
			{
				this->foreachNode(func, mRoot->mChildren[i], transform);
			}
		}
	}

	void Model::foreachNode(const std::function<Transform* (ModelNode*)>& func, ModelNode* node, Transform* parent)
	{
		auto transform = func(node);
		transform->setParent(parent);
		if (node->mChildrenCount > 0)
		{
			for (uint32 i = 0; i < node->mChildrenCount; i++)
			{
				this->foreachNode(func, node->mChildren[i], transform);
			}
		}
	}

	void Model::load(const std::string& path)
	{
		if (mRoot)
		{
			return;
		}

		uint32 load_flag = aiProcess_CalcTangentSpace | aiProcess_Triangulate
			| aiProcess_JoinIdenticalVertices | aiProcess_SortByPType
			| aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_RemoveComponent
			| aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_SplitLargeMeshes;

		uint32 remove_flag = aiComponent_LIGHTS | aiComponent_CAMERAS | aiComponent_MATERIALS;

		Assimp::Importer importer;

		importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, remove_flag);
		importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);

		const aiScene* ai_scene = importer.ReadFile(path, load_flag);

		if (ai_scene == nullptr)
		{
			return;
		}

		std::function<void(const aiScene*, aiNode*, ModelNode*, uint32 index)> func =
			[this, &func](const aiScene* aiscene, aiNode* ainode, ModelNode* parent, uint32 index)
		{
			auto mnode = this->createModelNode(aiscene, ainode);
			if (parent != nullptr)
			{
				parent->addChild(mnode);
			}

			if (ainode->mNumChildren > 0)
			{
				for (uint32 i = 0; i < ainode->mNumChildren; i++)
				{
					func(aiscene, ainode->mChildren[i], mnode, i);
				}
			}
		};

		auto ai_node = ai_scene->mRootNode;
		mRoot = this->createModelNode(ai_scene, ai_node);
		if (ai_node->mNumChildren > 0)
		{
			for (uint32 i = 0; i < ai_node->mNumChildren; i++)
			{
				func(ai_scene, ai_node->mChildren[i], mRoot, i);
			}
		}
	}

	ModelNode* Model::createModelNode(const aiScene* aiscene, aiNode* ainode)
	{
		ModelNode* mode_node = new ModelNode();
		mode_node->mName = ainode->mName.C_Str();
		mode_node->init(ainode->mNumMeshes, ainode->mNumChildren);

		auto& transform = ainode->mTransformation;

		//如果只有一个mesh,说明自身是mesh
		if (ainode->mNumMeshes == 1)
		{
			auto ai_mesh = aiscene->mMeshes[ainode->mMeshes[0]];
			auto mesh_data = this->createMesh(ai_mesh, ainode);

			auto vertex = Vertex::create();
			vertex->setMesh(mesh_data);
			vertex->generate();
			mode_node->setVertex(vertex);
		}
		else if (ainode->mNumMeshes > 1)
		{
			//如果不止一个mesh
			//把此层提升成父级
			//把meshes和children合并成一层
			//单独的mesh没有children
			for (uint32 i = 0; i < ainode->mNumMeshes; i++)
			{
				auto ai_mesh = aiscene->mMeshes[ainode->mMeshes[i]];
				auto mesh_data = this->createMesh(ai_mesh, ainode);

				auto vertex = Vertex::create();
				vertex->setMesh(mesh_data);
				vertex->generate();

				ModelNode* child = new ModelNode();
				child->mName = ai_mesh->mName.C_Str();
				child->init(1, 0);
				child->setVertex(vertex);

				mode_node->addChild(child);
			}
		}

		return mode_node;
	}

	MeshData* Model::createMesh(aiMesh* aimesh, const aiNode* node)
	{
		MeshData* meshData = MeshData::create(aimesh->mName.C_Str());

		meshData->mVertices.reserve(aimesh->mNumVertices);
		meshData->mNormals.reserve(aimesh->mNumVertices);
		meshData->mUVs.reserve(aimesh->mNumVertices);
		meshData->mColors.reserve(aimesh->mNumVertices);
		meshData->mTangents.reserve(aimesh->mNumVertices);
		meshData->mBitTangents.reserve(aimesh->mNumVertices);

		bool has_normal = aimesh->HasNormals();
		bool has_uv0 = aimesh->HasTextureCoords(0);
		bool has_color0 = aimesh->HasVertexColors(0);
		bool has_tangents = aimesh->HasTangentsAndBitangents();
		auto& transform = node->mTransformation;

		for (uint32 ver_i = 0; ver_i < aimesh->mNumVertices; ver_i++)
		{
			auto& ai_vertex = aimesh->mVertices[ver_i];
			auto v = transform * ai_vertex;
			meshData->mVertices.emplace_back(v.x, v.y, v.z);

			if (has_normal)
			{
				auto& ai_normal = aimesh->mNormals[ver_i];
				meshData->mNormals.emplace_back(ai_normal.x
										  , ai_normal.y
										  , ai_normal.z);
			}

			if (has_uv0)
			{
				auto& ai_uv = aimesh->mTextureCoords[0][ver_i];
				meshData->mUVs.emplace_back(ai_uv.x, ai_uv.y);
			}

			if (has_color0)
			{
				auto& ai_color = aimesh->mColors[0][ver_i];
				meshData->mColors.emplace_back(ai_color.r
										 , ai_color.g
										 , ai_color.b
										 , ai_color.a);
			}

			if (has_tangents)
			{
				auto& ai_tangents = aimesh->mTangents[ver_i];
				meshData->mTangents.emplace_back(ai_tangents.x
										   , ai_tangents.y
										   , ai_tangents.z);

				auto& ai_bit = aimesh->mBitangents[ver_i];
				meshData->mBitTangents.emplace_back(ai_bit.x
											  , ai_bit.y
											  , ai_bit.z);
			}
		}

		if (aimesh->HasFaces())
		{
			for (uint32 face_i = 0; face_i < aimesh->mNumFaces; face_i++)
			{
				auto& ai_face = aimesh->mFaces[face_i];
				for (uint32 index_i = 0; index_i < ai_face.mNumIndices; index_i++)
				{
					meshData->mIndices.emplace_back(ai_face.mIndices[index_i]);
				}
			}
		}

		meshData->apply();
		return meshData;
	}

	void Model::generate()
	{
		auto shader = ShaderManager::find("Standard/PBRTest1");
		auto index_albedo = shader->getUniformIndex("myPBR.albedo");
		auto index_metallic = shader->getUniformIndex("myPBR.metallic");
		auto index_roughness = shader->getUniformIndex("myPBR.roughness");
		auto index_ao = shader->getUniformIndex("myPBR.ao");

		this->foreachNode([=](ModelNode* node)
		{
			auto go = GameObject::create(node->mName);
			auto transform = go->addComponent<Transform>();
			if (node->mVertex)
			{
				auto mr = go->addComponent<MeshRenderer>();
				mr->setMesh(node->mVertex);

				auto material = Material::create(shader);
				mr->setMaterial(material);

				material->setUniform<UniformF3>(index_albedo, float3(1.0f, 1.0f, 1.0f));
				material->setUniform<UniformF1>(index_metallic, 0.8);
				material->setUniform<UniformF1>(index_roughness, 0.3);
				material->setUniform<UniformF1>(index_ao, 1.0f);
			}

			return transform;
		});
	}

}
