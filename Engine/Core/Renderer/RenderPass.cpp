#include "RenderPass.h"
#include "../Pipeline/PassVertexGroup.h"

#include "../Shader/Shader.h"
#include "../Component/Camera.h"
#include "../Component/Transform.h"
#include "../Component/MeshRenderer.h"
#include "../Component/Light.h"
#include "../Head/GLMHead.h"
#include "../Manager/LightManager.h"
#include "../Statistic.h"
#include "../Renderer/VertexGroup.h"


namespace tezcat::Tiny::Core
{
	RenderPass::RenderPass(Shader* shader)
		: RenderPass(shader, nullptr)
	{

	}

	RenderPass::RenderPass(Shader* shader, ILight* light)
		: RenderPass(shader, light, DrawMode::Triangles)
	{

	}

	RenderPass::RenderPass(Shader* shader, ILight* light, DrawMode drawMode)
		: mShader(shader)
		, mLight(light)
		, mDrawMode(ContextMap::DrawModeArray[(uint32_t)drawMode])
	{

	}

	RenderPass::~RenderPass()
	{
		mShader = nullptr;
	}

	void RenderPass::addRenderObject(MeshRenderer* meshRenderer)
	{
		mRenderObjects.push_back(meshRenderer);

		// 		auto vao = meshRenderer->getVertexGroup();
		// 		auto it = mVAOWithID.find(vao->getUID());
		// 		if (it == mVAOWithID.end())
		// 		{
		// 			auto pass = new PassVertexGroup(vao);
		// 			pass->addMeshRenderer(meshRenderer);
		// 			mVAOWithID.emplace(vao, pass);
		// 		}
		// 		else
		// 		{
		// 			it->second->addMeshRenderer(meshRenderer);
		// 		}
	}

	void RenderPass::render(Camera* camera)
	{
		//#PassShaderRender
// 		if (mVAOWithID.empty())
// 		{
// 			return;
// 		}

		if (mRenderObjects.empty())
		{
			return;
		}

		Statistic::PassCount += 1;
		Statistic::DrawCall += static_cast<int>(mRenderObjects.size());

		mShader->setStateOptions();
		// the same shader
		mShader->bind();

		if (camera != nullptr)
		{
			camera->submit(mShader);
		}

		//实际上应该是shader自己去找它需要的灯光数据
		//而不是灯光传数据
		if (mLight)
		{
			mLight->submit(mShader);
		}

		for (auto ro : mRenderObjects)
		{
			ro->submit(mShader);
		}

		mRenderObjects.clear();

		// 		auto it = mVAOWithID.begin();
		// 		while (it != mVAOWithID.end())
		// 		{
		// 			auto vao_pass = (*it++).second;
		// 			if (!vao_pass->empty())
		// 			{
		// 				vao_pass->render(mShader);
		// 			}
		// 		}
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
