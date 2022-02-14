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
		m_VertexGroupDic.emplace(vg->getName(), vg);
	}

	void VertexGroupManager::addVertexGroup(MeshData* meshData)
	{
		auto vg = Graphics->createVertexGroup(meshData);
		this->addVertexGroup(vg);
	}

	VertexGroup* VertexGroupManager::getVertexGroup(const std::string& name)
	{
		return m_VertexGroupDic[name];
	}
}
