#include "RenderPass.h"
#include "../Pipeline/PassVertexGroup.h"
#include "../Head/GLMHead.h"

#include "../Shader/Shader.h"
#include "../Component/Camera.h"
#include "../Component/Transform.h"
#include "../Component/MeshRenderer.h"
#include "../Component/Light.h"
#include "../Manager/LightManager.h"
#include "../Renderer/VertexGroup.h"
#include "../Renderer/BaseGraphics.h"
#include "../Renderer/RenderObject.h"

#include "../Statistic.h"


namespace tezcat::Tiny::Core
{
	RenderPass::RenderPass(Shader* shader)
		: mShader(shader)
	{

	}

	RenderPass::~RenderPass()
	{
		mShader = nullptr;
	}

	void RenderPass::addRenderMesh(IRenderMesh* renderMesh)
	{
		mRenderObjects.push_back(renderMesh);
	}

	void RenderPass::sortRenderObjects(const std::function<bool(IRenderMesh* a, IRenderMesh* b)>& function)
	{
		std::sort(mRenderObjects.begin(), mRenderObjects.end(), function);
	}

	void RenderPass::render(BaseGraphics* graphics, IRenderObserver* renderObserver)
	{
		if (mRenderObjects.empty())
		{
			return;
		}

		Statistic::PassCount += 1;
		Statistic::DrawCall += static_cast<int>(mRenderObjects.size());

		mShader->setStateOptions();
		// the same shader
		mShader->bind();

		//Observer which render this pass
		if (renderObserver != nullptr)
		{
			renderObserver->submit(mShader);
		}

		auto dir_light = LightMgr::getInstance()->getDirectionalLight();
		if (dir_light != nullptr)
		{
			dir_light->submit(mShader);
		}


		for (auto ro : mRenderObjects)
		{
			ro->beginRender();
			ro->submit(mShader);
			graphics->draw(ro);
			ro->endRender();
		}

		mRenderObjects.clear();
	}

	void RenderPass::renderShadowMap(BaseGraphics* graphics, IRenderObserver* renderObserver)
	{
		if (mRenderObjects.empty())
		{
			return;
		}

		Statistic::PassCount += 1;
		Statistic::DrawCall += static_cast<int>(mRenderObjects.size());

		mShader->setStateOptions();
		// the same shader
		mShader->bind();

		//Observer which render this pass
		if (renderObserver != nullptr)
		{
			renderObserver->submit(mShader);
		}

		//相机的matrix他自己已经先传了
		for (auto ro : mRenderObjects)
		{
			ro->beginRender();
			mShader->setModelMatrix(ro->getModelMatrix());
			graphics->draw(ro);
			ro->endRender();
		}

		mRenderObjects.clear();
	}

	int RenderPass::getOrderID() const
	{
		return mShader->getOrderID();
	}

	int RenderPass::getProgramID() const
	{
		return mShader->getProgramID();
	}

	const std::string& RenderPass::getName() const
	{
		return mShader->getName();
	}

	Shader* RenderPass::getShader()
	{
		return mShader;
	}
}
