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

#include "Core/Manager/ModelManager.h"
#include "Core/Manager/SceneManager.h"
#include "Core/Manager/VertexBufferManager.h"

#include "Core/Data/MeshData.h"

#include "Core/Component/Transform.h"
#include "Core/Component/MeshRenderer.h"
#include "Core/Component/GameObject.h"

#include "ThirdParty/assimp/Importer.hpp"
#include "ThirdParty/assimp/scene.h"
#include "ThirdParty/assimp/postprocess.h"
#include "ThirdParty/Hash/city.h"


namespace tezcat::Tiny
{
	std::vector<MeshData*> ModelManager::mMeshDataAry;
	std::unordered_map<uint64, ModelNode*> ModelManager::mModelUMap;

	void ModelManager::loadModel(const std::string& path)
	{
		if (SceneManager::isEmpty())
		{
			return;
		}

		auto hash_id = CityHash64(path.data(), path.size());
		auto result = mModelUMap.find(hash_id);
		if (result == mModelUMap.end())
		{
			auto current_scene = SceneManager::getCurrentScene();

			uint32_t load_flag = aiProcess_CalcTangentSpace | aiProcess_Triangulate
				| aiProcess_JoinIdenticalVertices | aiProcess_SortByPType
				| aiProcess_GenSmoothNormals | aiProcess_FlipUVs
				| aiProcess_RemoveComponent | aiProcess_OptimizeGraph;

			uint32_t remove_flag = aiComponent_LIGHTS | aiComponent_CAMERAS | aiComponent_MATERIALS;

			Assimp::Importer importer;

			importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, remove_flag);
			importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);

			const aiScene* ai_scene = importer.ReadFile(path, load_flag);

			if (ai_scene == nullptr)
			{
				return;
			}

			std::function<void(const aiScene*, aiNode*, ModelNode*, uint32_t index)> func =
				[func](const aiScene* aiscene, aiNode* ainode, ModelNode* parent, uint32_t index)
			{

			};

			auto ai_node = ai_scene->mRootNode;
			auto mnode = createModelNode(ai_scene, ai_node);
			mModelUMap.emplace(hash_id, mnode);
			if (ai_node->mNumChildren > 0)
			{
				for (uint32_t i = 0; i < ai_node->mNumChildren; i++)
				{
					func(ai_scene, ai_node->mChildren[i], mnode, i);
				}
			}
		}

		auto node = result->second;
	}

	ModelNode* ModelManager::createModelNode(const aiScene* aiscene, aiNode* ainode)
	{
		ModelNode* mnode = new ModelNode();
		mnode->mName = ainode->mName.C_Str();
		if (ainode->mNumMeshes > 0)
		{
			auto ai_mesh = aiscene->mMeshes[ainode->mMeshes[0]];
			auto mesh_data = createMesh(ai_mesh);
			VertexBufferManager::create(mesh_data);
		}

		return mnode;
	}

	Transform* ModelManager::createModel(const aiScene* aiscene, aiNode* ainode)
	{
		auto go = GameObject::create(ainode->mName.C_Str());
		auto transform = go->addComponent<Transform>();
		if (ainode->mNumMeshes > 0)
		{
			auto mr = go->addComponent<MeshRenderer>();
			auto ai_mesh = aiscene->mMeshes[ainode->mMeshes[0]];
			mr->setMesh(createMesh(ai_mesh));
		}

		return transform;
	}

	MeshData* ModelManager::createMesh(aiMesh* aimesh)
	{
		MeshData* mesh_data = MeshData::create(aimesh->mName.C_Str());
		mesh_data->mColors = new std::vector<float4>();
		mesh_data->mUVs = new std::vector<float2>();
		mesh_data->mNormals = new std::vector<float3>();
		mesh_data->mTangents = new std::vector<float3>();
		mesh_data->mBitTangents = new std::vector<float3>();

		mesh_data->mVertices.reserve(aimesh->mNumVertices);
		mesh_data->mNormals->reserve(aimesh->mNumVertices);
		mesh_data->mUVs->reserve(aimesh->mNumVertices);
		mesh_data->mColors->reserve(aimesh->mNumVertices);
		mesh_data->mTangents->reserve(aimesh->mNumVertices);
		mesh_data->mBitTangents->reserve(aimesh->mNumVertices);

		bool has_normal = aimesh->HasNormals();
		bool has_uv0 = aimesh->HasTextureCoords(0);
		bool has_color0 = aimesh->HasVertexColors(0);
		bool has_tangents = aimesh->HasTangentsAndBitangents();

		for (uint32_t ver_i = 0; ver_i < aimesh->mNumVertices; ver_i++)
		{
			auto ai_vertex = aimesh->mVertices[ver_i];
			mesh_data->mVertices.emplace_back(ai_vertex.x
									   , ai_vertex.y
									   , ai_vertex.z);

			if (has_normal)
			{
				auto ai_normal = aimesh->mNormals[ver_i];
				mesh_data->mNormals->emplace_back(ai_normal.x
										  , ai_normal.y
										  , ai_normal.z);
			}

			if (has_uv0)
			{
				auto ai_uv = aimesh->mTextureCoords[0][ver_i];
				mesh_data->mUVs->emplace_back(ai_uv.x, ai_uv.y);
			}

			if (has_color0)
			{
				auto ai_color = aimesh->mColors[0][ver_i];
				mesh_data->mColors->emplace_back(ai_color.r
										 , ai_color.g
										 , ai_color.b
										 , ai_color.a);
			}

			if (has_tangents)
			{
				auto ai_tangents = aimesh->mTangents[ver_i];
				mesh_data->mTangents->emplace_back(ai_tangents.x
										   , ai_tangents.y
										   , ai_tangents.z);

				auto ai_bit = aimesh->mBitangents[ver_i];
				mesh_data->mBitTangents->emplace_back(ai_bit.x
											  , ai_bit.y
											  , ai_bit.z);
			}
		}

		if (aimesh->HasFaces())
		{
			mesh_data->mIndices = new std::vector<uint32_t>();

			for (uint32_t face_i = 0; face_i < aimesh->mNumFaces; face_i++)
			{
				auto& ai_face = aimesh->mFaces[face_i];
				for (uint32_t index_i = 0; index_i < ai_face.mNumIndices; index_i++)
				{
					mesh_data->mIndices->emplace_back(ai_face.mIndices[index_i]);
				}

				if (ai_face.mNumIndices == 1)
				{
					mesh_data->mDrawMode = DrawMode::Points;
				}
				else if (ai_face.mNumIndices == 2)
				{
					mesh_data->mDrawMode = DrawMode::Lines;
				}
				else if (ai_face.mNumIndices == 3)
				{
					mesh_data->mDrawMode = DrawMode::Triangles;
				}
			}
		}

		return mesh_data;
	}
}

