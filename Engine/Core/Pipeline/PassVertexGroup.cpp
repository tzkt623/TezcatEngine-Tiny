#include "PassVertexGroup.h"
#include "../Renderer/RenderPass.h"
#include "../Renderer/VertexGroup.h"
#include "../Statistic.h"
#include "../Shader/Shader.h"
#include "../Component/MeshRenderer.h"
#include "../Renderer/BaseGraphics.h"

namespace tezcat::Tiny::Core
{
	PassVertexGroup::PassVertexGroup(VertexGroup* vg)
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

	void PassVertexGroup::render(Shader* shader)
	{
		// 同一个VAO
		mDelegateVertexGroup->bind();
		Statistic::DrawCall += static_cast<int>(mMeshRendererList.size());

		for (auto renderer : mMeshRendererList)
		{
			renderer->submit(shader);
			Graphics::getInstance()->draw(renderer);
		}

		mMeshRendererList.clear();
	}

	void PassVertexGroup::render(Shader* shader, DrawModeWrapper& drawMode)
	{
		// 同一个VAO
		mDelegateVertexGroup->bind();
		Statistic::DrawCall += static_cast<int>(mMeshRendererList.size());

		for (auto renderer : mMeshRendererList)
		{
			renderer->submit(shader);
			Graphics::getInstance()->draw(renderer->getVertexGroup(), drawMode);
		}

		mMeshRendererList.clear();
	}
}
