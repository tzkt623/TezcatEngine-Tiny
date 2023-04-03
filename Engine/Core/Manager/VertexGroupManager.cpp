#include "VertexGroupManager.h"
#include "../Renderer/VertexGroup.h"
#include "../Renderer/BaseGraphics.h"

namespace tezcat::Tiny::Core
{
	VertexGroupManager::VertexGroupManager()
	{
		VertexGroupMgr::attach(this);
	}

	VertexGroupManager::~VertexGroupManager()
	{

	}

	void VertexGroupManager::addVertexGroup(VertexGroup* vg)
	{
		mVertexGroupMap.emplace(vg->getName(), vg);
	}

	void VertexGroupManager::addVertexGroup(MeshData* meshData)
	{
		auto vg = VertexGroupCreator::create(meshData);
		this->addVertexGroup(vg);
	}

	VertexGroup* VertexGroupManager::getVertexGroup(const std::string& name)
	{
		return mVertexGroupMap[name];
	}
}
