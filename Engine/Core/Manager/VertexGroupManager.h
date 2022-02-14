#pragma once
#include "../Head/CppHead.h"
#include "Utility/Tools.h"

namespace tezcat::Tiny::Core
{
	class VertexGroup;
	class MeshData;
	class VertexGroupManager
	{
	public:
		VertexGroupManager();
		~VertexGroupManager();

	public:
		void addVertexGroup(VertexGroup* vao);
		void addVertexGroup(MeshData* meshData);
		VertexGroup* getVertexGroup(const std::string& name);

	private:
		std::unordered_map<std::string, VertexGroup*> m_VertexGroupDic;
	};

	using VertexGroupMgr = SG<VertexGroupManager>;
}