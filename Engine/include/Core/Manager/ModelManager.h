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
		static std::unordered_map<uint64_t, ModelNode*> mModelUMap;
		static std::vector<MeshData*> mMeshDataAry;
	};
}
