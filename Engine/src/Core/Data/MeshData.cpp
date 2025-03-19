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

#include "Core/Manager/ResourceManager.h"
#include "Core/Manager/ShaderManager.h"

#include "Core/Shader/Shader.h"

#include "Core/Debug/Debug.h"

#include "ThirdParty/assimp/Importer.hpp"
#include "ThirdParty/assimp/scene.h"
#include "ThirdParty/assimp/postprocess.h"
#include "ThirdParty/Hash/city.h"

namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(MeshData, TinyObject);
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
		: mVertex(nullptr)
		, mIndex(0)
		, mParentIndex(-1)
		, mTextureMap(nullptr)
		, mIsEnableBlend(false)
	{

	}

	ModelNode::~ModelNode()
	{
		if (mVertex)
		{
			mVertex->deleteObject();
			mVertex = nullptr;
		}

		if (mTextureMap)
		{
			delete mTextureMap;
			mTextureMap = nullptr;
		}

		if (mValueMap)
		{
			delete mValueMap;
			mValueMap = nullptr;
		}
	}

	void ModelNode::setVertex(Vertex* vertex)
	{
		mVertex = vertex;
		mVertex->saveObject();
	}

	std::unordered_map<MaterialTextureSlot, std::string>& ModelNode::getTextureMap()
	{
		if (mTextureMap == nullptr)
		{
			mTextureMap = new std::unordered_map<MaterialTextureSlot, std::string>();
		}

		return *mTextureMap;
	}

	std::tuple<bool, std::string_view> ModelNode::getTextureMap(MaterialTextureSlot slot)
	{
		auto result = mTextureMap->find(slot);
		return { result != mTextureMap->end(), result->second };
	}

	std::unordered_map<MaterialTextureSlot, ModelNode::Value>& ModelNode::getValueMap()
	{
		if (mValueMap == nullptr)
		{
			mValueMap = new std::unordered_map<MaterialTextureSlot, ModelNode::Value>();
		}

		return *mValueMap;
	}

	std::tuple<bool, ModelNode::Value> ModelNode::getValueMap(MaterialTextureSlot slot)
	{
		auto result = mValueMap->find(slot);
		return { result != mValueMap->end(), result->second };
	}

	void ModelNode::setBlend(Blend source, Blend target)
	{
		mSource = source;
		mTarget = target;
	}

	ModelNode::Value* ModelNode::tryGetValue(MaterialTextureSlot slot)
	{
		if (mValueMap == nullptr)
		{
			return nullptr;
		}

		auto it = mValueMap->find(slot);
		if (it == mValueMap->end())
		{
			return nullptr;
		}
		else
		{
			return &it->second;
		}
	}

	std::string* ModelNode::tryGetTexture(MaterialTextureSlot slot)
	{
		if (mTextureMap == nullptr)
		{
			return nullptr;
		}

		auto it = mTextureMap->find(slot);
		if (it == mTextureMap->end())
		{
			return nullptr;
		}
		else
		{
			return &it->second;
		}
	}


	TINY_OBJECT_CPP(Model, TinyObject);
	Model::Model()
		: mName()
	{

	}

	Model::~Model()
	{

	}

	bool Model::load(const file_path& path)
	{
		mPath = path;
		uint32_t load_flag = aiProcess_CalcTangentSpace
			| aiProcess_Triangulate
			| aiProcess_JoinIdenticalVertices
			| aiProcess_SortByPType
			| aiProcess_GenSmoothNormals
			//| aiProcess_FlipUVs			//DX需要这一段,GL不需要
			| aiProcess_RemoveComponent
			//| aiProcess_OptimizeMeshes	//当前参数执行后会自动优化mesh个数
			| aiProcess_OptimizeGraph
			| aiProcess_SplitLargeMeshes
			;

		uint32_t remove_flag = aiComponent_LIGHTS
			| aiComponent_CAMERAS
			//| aiComponent_MATERIALS
			;

		Assimp::Importer importer;

		importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, remove_flag);
		importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);

		const aiScene* ai_scene = importer.ReadFile(mPath.string(), load_flag);

		if (ai_scene == nullptr)
		{
			TINY_LOG_WARNING(std::format("Can`t load this model"));
			return false;
		}

		mChildren.reserve(ai_scene->mNumMeshes);
		this->createModelNode(ai_scene, ai_scene->mRootNode, -1);

		return true;
	}

	void Model::createModelNode(const aiScene* aiscene, aiNode* ainode, int32_t parentIndex)
	{
		ModelNode* this_layer = nullptr;

		//如果这一层有多个mesh
		//说明需要分割这一层让其变成父节点
		//他其中的mesh分别作为子节点链接至此层
		if (ainode->mNumMeshes > 1)
		{
			this_layer = new ModelNode();
			this_layer->mParentIndex = parentIndex;
			this_layer->mIndex = mChildren.size();
			this_layer->mName = ainode->mName.C_Str();
			mChildren.push_back(this_layer);

			for (uint32_t i = 0; i < ainode->mNumMeshes; i++)
			{
				auto child_mesh = aiscene->mMeshes[ainode->mMeshes[i]];
				auto mesh_data = this->createMesh(child_mesh, aiscene, ainode);

				auto vertex = Vertex::create();
				vertex->setMesh(mesh_data);
				vertex->generate();

				ModelNode* child = new ModelNode();
				child->mName = child_mesh->mName.C_Str();
				child->mParentIndex = this_layer->mIndex;
				child->setVertex(vertex);
				this->setMaterial(child, aiscene->mMaterials[child_mesh->mMaterialIndex]);

				child->mIndex = mChildren.size();
				mChildren.push_back(child);
			}
		}
		//说明当前层本身就是mesh
		else if (ainode->mNumMeshes == 1)
		{
			auto mesh = aiscene->mMeshes[ainode->mMeshes[0]];
			auto mesh_data = this->createMesh(mesh, aiscene, ainode);

			auto vertex = Vertex::create();
			vertex->setMesh(mesh_data);
			vertex->generate();

			this_layer = new ModelNode();
			this_layer->mName = mesh->mName.C_Str();
			this_layer->mParentIndex = parentIndex;
			this_layer->setVertex(vertex);
			this->setMaterial(this_layer, aiscene->mMaterials[mesh->mMaterialIndex]);

			this_layer->mIndex = mChildren.size();
			mChildren.push_back(this_layer);
		}
		else
		{
			this_layer = new ModelNode();
			this_layer->mParentIndex = parentIndex;
			this_layer->mIndex = mChildren.size();
			this_layer->mName = ainode->mName.C_Str();
			mChildren.push_back(this_layer);
		}

		for (uint32_t i = 0; i < ainode->mNumChildren; i++)
		{
			this->createModelNode(aiscene, ainode->mChildren[i], this_layer->mIndex);
		}
	}

	MeshData* Model::createMesh(aiMesh* aimesh, const aiScene* aiscene, const aiNode* node)
	{
		auto material = aiscene->mMaterials[aimesh->mMaterialIndex];

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

		aiColor4D diffuseColor(0.0f, 0.0f, 0.0f, 1.f);

		for (uint32_t ver_i = 0; ver_i < aimesh->mNumVertices; ver_i++)
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

			if (material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == aiReturn_SUCCESS)
			{
				meshData->mColors.emplace_back(diffuseColor.r
					, diffuseColor.g
					, diffuseColor.b
					, diffuseColor.a);
			}
			else if (has_color0)
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
			for (uint32_t face_i = 0; face_i < aimesh->mNumFaces; face_i++)
			{
				auto& ai_face = aimesh->mFaces[face_i];
				for (uint32_t index_i = 0; index_i < ai_face.mNumIndices; index_i++)
				{
					meshData->mIndices.emplace_back(ai_face.mIndices[index_i]);
				}
			}
		}

		meshData->apply();
		return meshData;
	}

	GameObject* Model::generate()
	{
		std::vector<GameObject*> go_array;
		go_array.reserve(mChildren.size());

		//创建所有go
		for (uint32_t i = 0; i < mChildren.size(); i++)
		{
			auto node = mChildren[i];
			auto go = GameObject::create(node->mName);
			auto transform = go->addComponent<Transform>();
			if (node->mVertex)
			{
				auto mr = go->addComponent<MeshRenderer>();
				mr->setMesh(node->mVertex);

				if (node->mIsPBR)
				{
					if (node->mHasTexture)
					{
						auto shader = ShaderManager::find("Standard/PBRStd1");
						auto material = Material::create(shader);
						mr->setMaterial(material);

						auto index_albedo = shader->getUniformIndex("myPBR.albedo2D");
						auto index_metallic = shader->getUniformIndex("myPBR.metallic2D");
						auto index_roughness = shader->getUniformIndex("myPBR.roughness2D");
						auto index_ao = shader->getUniformIndex("myPBR.ao2D");


						//albedo
						auto tex_path = node->tryGetTexture(MaterialTextureSlot::PBR_TEX_BASE_COLOR);
						if (tex_path && std::filesystem::exists(*tex_path))
						{
							material->setUniform<UniformTex2D>(index_albedo, ResourceManager::loadAndSave<Texture2D>(*tex_path));
						}
						else
						{
							material->setUniform<UniformTex2D>(index_albedo, ResourceManager::loadDefault<Texture2D>("Image/Tiny/TinyOne.png"));
						}

						//
						tex_path = node->tryGetTexture(MaterialTextureSlot::PBR_TEX_METALNESS);
						if (tex_path && std::filesystem::exists(*tex_path))
						{
							material->setUniform<UniformTex2D>(index_metallic, ResourceManager::loadAndSave<Texture2D>(*tex_path));
						}
						else
						{
							material->setUniform<UniformTex2D>(index_metallic, ResourceManager::loadDefault<Texture2D>("Image/Tiny/TinyZero.png"));
						}

						tex_path = node->tryGetTexture(MaterialTextureSlot::PBR_TEX_ROUGHNESS);
						if (tex_path && std::filesystem::exists(*tex_path))
						{
							material->setUniform<UniformTex2D>(index_roughness, ResourceManager::loadAndSave<Texture2D>(*tex_path));
						}
						else
						{
							material->setUniform<UniformTex2D>(index_roughness, ResourceManager::loadDefault<Texture2D>("Image/Tiny/TinyZero.png"));
						}

						tex_path = node->tryGetTexture(MaterialTextureSlot::PBR_TEX_AMBIENT_OCCLUSION);
						if (tex_path && std::filesystem::exists(*tex_path))
						{
							material->setUniform<UniformTex2D>(index_ao, ResourceManager::loadAndSave<Texture2D>(*tex_path));
						}
						else
						{
							material->setUniform<UniformTex2D>(index_ao, ResourceManager::loadDefault<Texture2D>("Image/Tiny/TinyZero.png"));
						}

						//material->setUniform<UniformTex2D>(index_albedo
						//	, ResourceManager::loadAndSave<Texture2D>(node->getTextureMap().at(MaterialTextureSlot::PBR_TEX_BASE_COLOR)));
						//material->setUniform<UniformTex2D>(index_metallic
						//, ResourceManager::loadAndSave<Texture2D>(node->getTextureMap().at(MaterialTextureSlot::PBR_TEX_METALNESS)));
						//material->setUniform<UniformTex2D>(index_roughness
						//, ResourceManager::loadAndSave<Texture2D>(node->getTextureMap().at(MaterialTextureSlot::PBR_TEX_ROUGHNESS)));
						//material->setUniform<UniformTex2D>(index_ao
						//, ResourceManager::loadAndSave<Texture2D>(node->getTextureMap().at(MaterialTextureSlot::PBR_TEX_AMBIENT_OCCLUSION)));
					}
					else
					{
						auto shader = ShaderManager::find("Standard/PBRTest1");
						auto material = Material::create(shader);
						mr->setMaterial(material);

						auto index_albedo = shader->getUniformIndex("myPBR.albedo");
						auto index_metallic = shader->getUniformIndex("myPBR.metallic");
						auto index_roughness = shader->getUniformIndex("myPBR.roughness");
						auto index_ao = shader->getUniformIndex("myPBR.ao");

						auto& color_albedo = node->getValueMap().at(MaterialTextureSlot::PBR_BASE_COLOR);
						material->setUniform<UniformF3>(index_albedo
							, float3(color_albedo.color.r, color_albedo.color.g, color_albedo.color.b));

						material->setUniform<UniformF1>(index_metallic
							, node->getValueMap().at(MaterialTextureSlot::PBR_METALNESS).v1);
						material->setUniform<UniformF1>(index_roughness
							, node->getValueMap().at(MaterialTextureSlot::PBR_ROUGHNESS).v1);
						material->setUniform<UniformF1>(index_ao
							, node->getValueMap().at(MaterialTextureSlot::PBR_AMBIENT_OCCLUSION).v1);
					}
				}
				else
				{
					if (node->mHasTexture)
					{
						auto shader = ShaderManager::find("Standard/Std1");
						auto material = Material::create(shader);
						mr->setMaterial(material);

						auto index_diffuse = shader->getUniformIndex("myTexDiffuse2D");
						auto index_specular = shader->getUniformIndex("myTexSpecular2D");
						auto index_shininess = shader->getUniformIndex("myTexShininess2D");

						auto diff = node->tryGetTexture(MaterialTextureSlot::TEX_DIFFUSE);
						if (diff && std::filesystem::exists(*diff))
						{
							auto tex = ResourceManager::loadAndSave<Texture2D>(*diff);
							material->setUniform<UniformTex2D>(index_diffuse, tex);
						}
						else
						{
							auto tex = ResourceManager::loadDefault<Texture2D>("Image/Tiny/TinyOne.png");
							material->setUniform<UniformTex2D>(index_diffuse, tex);
						}

						auto spe = node->tryGetTexture(MaterialTextureSlot::TEX_SPECULA);
						if (spe && std::filesystem::exists(*diff))
						{
							auto tex = ResourceManager::loadAndSave<Texture2D>(*diff);
							material->setUniform<UniformTex2D>(index_specular, tex);
						}
						else
						{
							auto tex = ResourceManager::loadDefault<Texture2D>("Image/Tiny/TinyZero.png");
							material->setUniform<UniformTex2D>(index_specular, tex);
						}

						auto shi = node->tryGetTexture(MaterialTextureSlot::TEX_SHININESS);
						if (shi && std::filesystem::exists(*diff))
						{
							auto tex = ResourceManager::loadAndSave<Texture2D>(*shi);
							material->setUniform<UniformTex2D>(index_shininess, tex);
						}
						else
						{
							auto tex = ResourceManager::loadDefault<Texture2D>("Image/Tiny/TinyZero.png");
							material->setUniform<UniformTex2D>(index_shininess, tex);
						}
					}
					else
					{
						auto shader = ShaderManager::find("Standard/StdValue1");
						auto material = Material::create(shader);
						mr->setMaterial(material);

						auto index_diffuse = shader->getUniformIndex("myDiffuse");
						auto index_specular = shader->getUniformIndex("mySpecular");
						auto index_shininess = shader->getUniformIndex("myShininess");

						auto v1 = node->tryGetValue(MaterialTextureSlot::DIFFUSE);
						if (v1)
						{
							material->setUniform<UniformF3>(index_diffuse, float3(v1->color.r, v1->color.g, v1->color.b));
						}
						else
						{
							material->setUniform<UniformF3>(index_diffuse, float3(1.0f, 0.0f, 1.0f));
						}

						auto v2 = node->tryGetValue(MaterialTextureSlot::SPECULA);
						if (v2)
						{
							material->setUniform<UniformF3>(index_specular, float3(v2->color.r, v2->color.g, v2->color.b));
						}
						else
						{
							material->setUniform<UniformF3>(index_specular, float3(1.0f, 0.0f, 1.0f));
						}

						auto v3 = node->tryGetValue(MaterialTextureSlot::SHININESS);
						if (v3)
						{
							material->setUniform<UniformF1>(index_shininess, v3->v1);
						}
						else
						{
							material->setUniform<UniformF1>(index_shininess, 0.0f);
						}
					}
				}
			}

			go_array.push_back(go);
		}

		//链接go
		for (uint32_t i = 0; i < mChildren.size(); i++)
		{
			auto node = mChildren[i];
			if (node->mParentIndex != -1)
			{
				go_array[i]->getTransform()->setParent(go_array[node->mParentIndex]->getTransform());
			}
		}

		return go_array[0];
	}

	template <typename Enum>
	constexpr auto EnumNameArray()
	{
		if constexpr (std::is_same_v<Enum, MaterialTextureSlot>)
		{
			return std::array
			{
				"AMBIENT",
				"TEX_AMBIENT",

				"DIFFUSE",
				"TEX_DIFFUSE",

				"SPECULA",
				"TEX_SPECULA",

				"NORMALS",
				"TEX_NORMALS",

				"EMISSIVE",
				"TEX_EMISSIVE",

				"SHININESS",
				"TEX_SHININESS",

				"REFLECTION",
				"TEX_REFLECTION",

				"TRANSPARENT",
				"TEX_TRANSPARENT",

				//PBR
				"PBR_BASE_COLOR",
				"PBR_TEX_BASE_COLOR",

				"PBR_NORMAL",
				"PBR_TEX_NORMAL",

				"PBR_ROUGHNESS",
				"PBR_TEX_ROUGHNESS",

				"PBR_EMISSION",
				"PBR_TEX_EMISSION",

				"PBR_AMBIENT_OCCLUSION",
				"PBR_TEX_AMBIENT_OCCLUSION",


				"PBR_METALNESS",
				"PBR_TEX_METALNESS",

				"PBR_GLOSSINESS",
				"PBR_TEX_GLOSSINESS",
			};
		}
	}

	template <typename Enum>
	constexpr const char* ConvertEnumToString(Enum value)
	{
		return EnumNameArray<Enum>()[static_cast<int32_t>(value)];
	}

#define TINY_BEGIN_ENUM

	void Model::setMaterial(ModelNode* node, aiMaterial* aiMaterial)
	{
		//this->setBlend(node, aiMaterial);
		this->setShading(node, aiMaterial);
	}

	void Model::setBlend(ModelNode* node, aiMaterial* aiMaterial)
	{
		aiBlendMode blend_mode = aiBlendMode_Default;
		if (aiMaterial->Get(AI_MATKEY_BLEND_FUNC, blend_mode) == aiReturn_SUCCESS)
		{
			node->mIsEnableBlend = true;
			switch (blend_mode)
			{
			case aiBlendMode_Default:
				node->setBlend(Blend::SourceAlpha, Blend::One_Minus_SourceAlpha);
				break;
			case aiBlendMode_Additive:
				node->setBlend(Blend::One, Blend::One);
				break;
			case _aiBlendMode_Force32Bit:
			default:
				node->setBlend(Blend::SourceAlpha, Blend::One_Minus_SourceAlpha);
				break;
			}
		}
	}

	void Model::setShading(ModelNode* node, aiMaterial* aiMaterial)
	{
		aiString texturePath;
		auto loadTexture = [&node, &aiMaterial, &texturePath](aiTextureType type, MaterialTextureSlot slot)
			{
				if (aiMaterial->GetTexture(type, 0, &texturePath) == aiReturn_SUCCESS)
				{
					node->getTextureMap().emplace(slot, texturePath.C_Str());
					//TINY_LOG_INFO(std::format("{} | {}", ConvertEnumToString(slot), texturePath.C_Str()));
				}
			};

		aiColor4D color;
		auto loadColor = [&node, &aiMaterial, &color](const char* key, unsigned int type, unsigned int idx, MaterialTextureSlot slot)
			{
				if (aiMaterial->Get(key, type, idx, color) == aiReturn_SUCCESS)
				{
					ModelNode::Value temp
					{
						color.r,
						color.g,
						color.b,
						color.a
					};
					node->getValueMap().emplace(slot, temp);
					//TINY_LOG_INFO(std::format("{} | R:{} G:{} B:{} A:{}", ConvertEnumToString(slot), temp.color.r, temp.color.g, temp.color.b, temp.color.a));
				}
			};

		float value = 0;
		auto loadValue = [&node, &aiMaterial, &value](const char* key, unsigned int type, unsigned int idx, MaterialTextureSlot slot)
			{
				if (aiMaterial->Get(key, type, idx, value) == aiReturn_SUCCESS)
				{
					ModelNode::Value temp{ .v1 = value };
					node->getValueMap().emplace(slot, temp);
					//TINY_LOG_INFO(std::format("{} | V:{}", ConvertEnumToString(slot), temp.v1));
				}
			};

		aiShadingMode shading_mode = aiShadingMode_NoShading;
		if (aiMaterial->Get(AI_MATKEY_SHADING_MODEL, shading_mode) == aiReturn_SUCCESS)
		{
			if (shading_mode == aiShadingMode_PBR_BRDF)
			{
				node->mIsPBR = true;
				if (AssimpHelper::isPBRTexture(aiMaterial))
				{
					node->mHasTexture = true;
					node->mShader = ShaderManager::find("Standard/PBRStd1");
					loadTexture(aiTextureType_BASE_COLOR, MaterialTextureSlot::PBR_TEX_BASE_COLOR);
					loadTexture(aiTextureType_NORMAL_CAMERA, MaterialTextureSlot::PBR_TEX_NORMAL);
					loadTexture(aiTextureType_EMISSION_COLOR, MaterialTextureSlot::PBR_TEX_EMISSION);
					loadTexture(aiTextureType_METALNESS, MaterialTextureSlot::PBR_TEX_METALNESS);
					loadTexture(aiTextureType_DIFFUSE_ROUGHNESS, MaterialTextureSlot::PBR_TEX_ROUGHNESS);
					loadTexture(aiTextureType_AMBIENT_OCCLUSION, MaterialTextureSlot::PBR_TEX_AMBIENT_OCCLUSION);
				}
				else
				{
					node->mHasTexture = false;
					node->mShader = ShaderManager::find("Standard/PBRTest1");
					loadColor(AI_MATKEY_BASE_COLOR, MaterialTextureSlot::PBR_BASE_COLOR);
					loadValue(AI_MATKEY_METALLIC_FACTOR, MaterialTextureSlot::PBR_METALNESS);
					loadValue(AI_MATKEY_GLOSSINESS_FACTOR, MaterialTextureSlot::PBR_GLOSSINESS);
					loadValue(AI_MATKEY_EMISSIVE_INTENSITY, MaterialTextureSlot::PBR_EMISSION);
				}

				return;
			}
		}


		node->mIsPBR = false;
		if (aiMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			node->mHasTexture = true;
			node->mShader = ShaderManager::find("Standard/Std1");
			loadTexture(aiTextureType_AMBIENT, MaterialTextureSlot::TEX_AMBIENT);
			loadTexture(aiTextureType_DIFFUSE, MaterialTextureSlot::TEX_DIFFUSE);
			loadTexture(aiTextureType_SPECULAR, MaterialTextureSlot::TEX_SPECULA);
			loadTexture(aiTextureType_NORMALS, MaterialTextureSlot::TEX_NORMALS);
			loadTexture(aiTextureType_EMISSIVE, MaterialTextureSlot::TEX_EMISSIVE);
			loadTexture(aiTextureType_SHININESS, MaterialTextureSlot::TEX_SHININESS);
			loadTexture(aiTextureType_REFLECTION, MaterialTextureSlot::TEX_REFLECTION);
			loadTexture(aiTextureType_OPACITY, MaterialTextureSlot::TEX_TRANSPARENT);
		}
		else
		{
			loadColor(AI_MATKEY_COLOR_AMBIENT, MaterialTextureSlot::AMBIENT);
			loadColor(AI_MATKEY_COLOR_DIFFUSE, MaterialTextureSlot::DIFFUSE);
			loadColor(AI_MATKEY_COLOR_SPECULAR, MaterialTextureSlot::SPECULA);
			loadColor(AI_MATKEY_COLOR_EMISSIVE, MaterialTextureSlot::EMISSIVE);
			loadColor(AI_MATKEY_COLOR_REFLECTIVE, MaterialTextureSlot::REFLECTION);
			loadColor(AI_MATKEY_COLOR_TRANSPARENT, MaterialTextureSlot::TRANSPARENT);
		}
	}

	bool AssimpHelper::isPBRValue(aiMaterial* material)
	{
		// 检查金属性 (Metallic)
		float value;
		if (material->Get(AI_MATKEY_METALLIC_FACTOR, value) == AI_SUCCESS)
		{
			return true;
		}

		// 检查粗糙度 (Roughness)
		if (material->Get(AI_MATKEY_ROUGHNESS_FACTOR, value) == AI_SUCCESS)
		{
			return true;
		}

		return false;
	}

	bool AssimpHelper::isPBRTexture(aiMaterial* material)
	{
		return material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) > 0
			|| material->GetTextureCount(aiTextureType_METALNESS) > 0;
	}

}
