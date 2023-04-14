#include "RenderPass.h"
#include "../Pipeline/PassVertexGroup.h"
#include "../Head/GLMHead.h"

#include "../Shader/Shader.h"
#include "../Manager/LightManager.h"
#include "../Pipeline/Pipeline.h"

#include "../Component/Camera.h"
#include "../Component/Transform.h"
#include "../Component/MeshRenderer.h"
#include "../Component/Light.h"

#include "../Renderer/VertexGroup.h"
#include "../Renderer/BaseGraphics.h"
#include "../Renderer/RenderObject.h"

#include "../Statistic.h"


namespace tezcat::Tiny::Core
{
	std::vector<RenderPass*> RenderPass::sPassAry;
	std::unordered_map<std::string, RenderPass*> RenderPass::sPassDict;


	RenderPass::RenderPass(Shader* shader)
		: mShader(shader)
	{
		Pipeline::addPassStatic(this);
	}

	RenderPass::~RenderPass()
	{
		mShader = nullptr;
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

	void RenderPass::addRenderMesh(IRenderMesh* renderMesh)
	{
		mRenderMeshList.push_back(renderMesh);
	}

	void RenderPass::sortRenderObjects(const std::function<bool(IRenderMesh* a, IRenderMesh* b)>& function)
	{
		std::sort(mRenderMeshList.begin(), mRenderMeshList.end(), function);
	}

	void RenderPass::render(BaseGraphics* graphics)
	{
		for (auto ro : mRenderMeshList)
		{
			ro->beginRender();
			ro->submitModelMatrix(mShader);
			ro->submit(mShader);
			graphics->draw(ro);
			ro->endRender();
		}

		mRenderMeshList.clear();
	}

	void RenderPass::renderMeshOnly(BaseGraphics* graphics)
	{
		for (auto ro : mRenderMeshList)
		{
			ro->beginRender();
			ro->submitModelMatrix(mShader);
			graphics->draw(ro);
			ro->endRender();
		}

		mRenderMeshList.clear();
	}
	//
	//
	//
	RenderPass* RenderPass::create(Shader* shader)
	{
		auto result = sPassDict.try_emplace(shader->getName(), nullptr);
		if (result.second)
		{
			while (sPassAry.size() <= shader->getUID())
			{
				sPassAry.push_back(nullptr);
			}
			auto pass = new RenderPass(shader);
			sPassAry[shader->getUID()] = pass;
			result.first->second = pass;
			return pass;
		}
		else
		{
			throw std::logic_error("Shader : the same name!!!!");
		}
	}

	RenderPass* RenderPass::get(Shader* shader)
	{
		return sPassAry[shader->getUID()];
	}

	RenderPass* RenderPass::get(const std::string& name)
	{
		auto it = sPassDict.find(name);
		if (it != sPassDict.end())
		{
			return (*it).second;
		}

		throw std::invalid_argument("No Pass Named: " + name);
	}

	bool RenderPass::checkState()
	{
		if (mRenderMeshList.empty())
		{
			return false;
		}

		mShader->setStateOptions();
		//The same shader
		mShader->bind();

		Statistic::PassCount += 1;
		Statistic::DrawCall += static_cast<int>(mRenderMeshList.size());

		return true;
	}

}
