#pragma once

#include "../Head/TinyCpp.h"
#include "../Tool/Tool.h"
#include "../Data/MeshData.h"

struct aiMesh;
struct aiNode;
struct aiScene;
namespace tezcat::Tiny
{
	class ModelNode;
	class Transform;
	class ModelManager
	{
	public:
		ModelManager();
		~ModelManager();

		void loadModel(const std::string& path);

		Transform* createModel(const aiScene* aiscene, aiNode* ainode);
		ModelNode* createModelNode(const aiScene* aiscene, aiNode* ainode);

		MeshData* createMesh(aiMesh* aimesh);

	private:
		std::unordered_map<uint64_t, ModelNode*> mModelUMap;
		std::vector<MeshData*> mMeshDataAry;
	};

	using ModelMgr = SG<ModelManager>;
}
