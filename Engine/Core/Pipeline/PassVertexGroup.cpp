#include "PassVertexGroup.h"
#include "../Renderer/RenderPass.h"
#include "../Renderer/Vertex.h"
#include "../Statistic.h"
#include "../Shader/Shader.h"
#include "../Component/MeshRenderer.h"
#include "../Renderer/BaseGraphics.h"

namespace tezcat::Tiny
{
	PassVertexGroup::PassVertexGroup(Vertex* vg)
		: mDelegateVertexGroup(vg)
	{

	}

	PassVertexGroup::~PassVertexGroup()
	{
	}

	void PassVertexGroup::addRenderObject(MeshRenderer* meshRenderer)
	{
		mMeshRendererList.push_back(meshRenderer);
	}
}
