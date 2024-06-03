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
		ModelManager() = delete;
		~ModelManager() = delete;
	public:
		static void loadModel(const std::string& path);

		static Transform* createModel(const aiScene* aiscene, aiNode* ainode);
		static ModelNode* createModelNode(const aiScene* aiscene, aiNode* ainode);

		static MeshData* createMesh(aiMesh* aimesh);

	private:
		static std::unordered_map<uint64, ModelNode*> mModelUMap;
		static std::vector<MeshData*> mMeshDataAry;
	};
}
