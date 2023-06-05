#include "ModelManager.h"
#include "VertexBufferManager.h"

#include "../Data/MeshData.h"

#include "ThirdParty/assimp/Importer.hpp"
#include "ThirdParty/assimp/scene.h"
#include "ThirdParty/assimp/postprocess.h"


namespace tezcat::Tiny
{
	ModelManager::ModelManager()
	{

	}

	ModelManager::~ModelManager()
	{

	}

	void ModelManager::loadModel(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* ai_scene = importer.ReadFile(path
												  , aiProcess_CalcTangentSpace
												  | aiProcess_Triangulate
												  | aiProcess_JoinIdenticalVertices
												  | aiProcess_SortByPType);

		if (ai_scene == nullptr)
		{
			return;
		}

		for (uint32_t i = 0; i < ai_scene->mNumMeshes; i++)
		{
			auto ai_mesh = ai_scene->mMeshes[i];

			ai_scene->mRootNode->FindNode(ai_mesh->mName);

			this->createMesh(ai_mesh);
		}

		auto ai_root = ai_scene->mRootNode;
		if (ai_root->mNumChildren > 0)
		{
			for (uint32_t i = 0; i < ai_root->mNumChildren; i++)
			{
				auto child = ai_root->mChildren[i];
				auto ai_meshes = child->mMeshes;
				//加载场景中的Mesh
				if (child->mNumMeshes > 0)
				{
					auto mRoot = new ModelNode();
					ModelNode* mesh_node = new ModelNode();
					mRoot->addChild(mesh_node);

					for (uint32_t mesh_i = 0; mesh_i < child->mNumMeshes; mesh_i++)
					{
						auto ai_mesh = ai_scene->mMeshes[ai_meshes[mesh_i]];
						this->createMesh(ai_mesh);
					}
				}
			}
		}
	}

	MeshData* ModelManager::createMesh(aiMesh* aimesh)
	{
		MeshData* meshData = MeshData::create(aimesh->mName.C_Str());
		mMeshDataAry.push_back(meshData);

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

		for (uint32_t ver_i = 0; ver_i < aimesh->mNumVertices; ver_i++)
		{
			auto ai_vertex = aimesh->mVertices[ver_i];
			meshData->mVertices.emplace_back(ai_vertex.x
									   , ai_vertex.y
									   , ai_vertex.z);

			if (has_normal)
			{
				auto ai_normal = aimesh->mNormals[ver_i];
				meshData->mNormals.emplace_back(ai_normal.x
										  , ai_normal.y
										  , ai_normal.z);
			}

			if (has_uv0)
			{
				auto ai_uv = aimesh->mTextureCoords[0][ver_i];
				meshData->mUVs.emplace_back(ai_uv.x, ai_uv.y);
			}

			if (has_color0)
			{
				auto ai_color = aimesh->mColors[0][ver_i];
				meshData->mColors.emplace_back(ai_color.r
										 , ai_color.g
										 , ai_color.b
										 , ai_color.a);
			}

			if (has_tangents)
			{
				auto ai_tangents = aimesh->mTangents[ver_i];
				meshData->mTangents.emplace_back(ai_tangents.x
										   , ai_tangents.y
										   , ai_tangents.z);

				auto ai_bit = aimesh->mBitangents[ver_i];
				meshData->mBitTangents.emplace_back(ai_bit.x
											  , ai_bit.y
											  , ai_bit.z);
			}
		}

		if (aimesh->HasFaces())
		{
			for (uint32_t face_i = 0; face_i < aimesh->mNumFaces; face_i++)
			{
				auto ai_face = aimesh->mFaces[face_i];
				for (uint32_t index_i = 0; index_i < ai_face.mNumIndices; index_i++)
				{
					meshData->mIndices.emplace_back(ai_face.mIndices[index_i]);
				}

				if (ai_face.mNumIndices == 1)
				{
					meshData->mDrawMode = DrawMode::Points;
				}
				else if (ai_face.mNumIndices == 2)
				{
					meshData->mDrawMode = DrawMode::Lines;
				}
				else if (ai_face.mNumIndices == 3)
				{
					meshData->mDrawMode = DrawMode::Triangles;
				}
			}
		}

		return meshData;
	}

}

