#include "PassVertexGroup.h"
#include "PassShader.h"
#include "../Renderer/VertexGroup.h"
#include "../Statistic.h"
#include "../Shader/Shader.h"
#include "../Component/MeshRenderer.h"
#include "../Renderer/BaseGraphics.h"

namespace tezcat::Tiny::Core
{
	PassVertexGroup::PassVertexGroup(VertexGroup* vg)
		: m_DelegateVertexGroup(vg)
	{

	}

	PassVertexGroup::~PassVertexGroup()
	{
	}

	void PassVertexGroup::addMeshRenderer(MeshRenderer* meshRenderer)
	{
		m_MeshRendererList.push_back(meshRenderer);
	}

	void PassVertexGroup::render(Shader* shader)
	{
		// 共用同一个VAO
		m_DelegateVertexGroup->bind();
		Statistic::DrawCall += static_cast<int>(m_MeshRendererList.size());

		for (auto renderer : m_MeshRendererList)
		{
			double time;
			TINY_PROFILER_TIMER_OUT(time);
			renderer->submit(shader);
			Graphics->renderMesh(renderer);
		}

		m_MeshRendererList.clear();
	}
}