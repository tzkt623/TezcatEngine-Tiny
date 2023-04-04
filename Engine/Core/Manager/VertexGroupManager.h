#pragma once
#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

#include "Utility/Utility.h"

namespace tezcat::Tiny::Core
{
	class VertexGroup;
	class MeshData;
	class TINY_API VertexGroupManager
	{
	public:
		VertexGroupManager();
		~VertexGroupManager();

	public:
		void addVertexGroup(VertexGroup* vao);
		void addVertexGroup(MeshData* meshData);
		VertexGroup* getVertexGroup(const std::string& name);

	private:
		std::unordered_map<std::string, VertexGroup*> mVertexGroupMap;
	};

	using VertexGroupMgr = SG<VertexGroupManager>;
}
