#pragma once

#include "../Head/TinyCpp.h"
#include "../Tool/Tool.h"
#include "../Data/MeshData.h"

struct aiMesh;
namespace tezcat::Tiny
{
	class Model;
	class ModelManager
	{
	public:
		ModelManager();
		~ModelManager();

		void loadModel(const std::string& path);

		MeshData* createMesh(aiMesh* aimesh);


	private:
		std::unordered_map<std::string, ModelNode*> mModelUMap;
		std::vector<MeshData*> mMeshDataAry;
	};

	using ModelMgr = SG<ModelManager>;
}
