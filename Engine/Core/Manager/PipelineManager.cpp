#include "PipelineManager.h"
#include "../Shader/Shader.h"
#include "../Renderer/RenderPass.h"
#include "../Renderer/BaseGraphics.h"
#include "../Layer/RenderLayer.h"
#include "../Layer/LightLayer.h"
#include "../Component/Light.h"
#include "../Pipeline/Forward.h"

namespace tezcat::Tiny::Core
{
	PipelineManager::PipelineManager()
	{
		mPassList.reserve(64);
		mRenderLayerList.reserve(32);
		for (int i = 0; i < 32; i++)
		{
			mRenderLayerList.push_back(new RenderLayer());
		}

		sPipelineMap.emplace("Forward", new Forward());

		SG<PipelineManager>::attach(this);
	}

	PipelineManager::~PipelineManager()
	{

	}

	void PipelineManager::createPass(Shader* shader)
	{
		while (mPassList.size() <= shader->getUID())
		{
			mPassList.push_back(nullptr);
		}

		auto pass = new RenderPass(shader);
		mPassList[shader->getUID()] = pass;
		sPipelineMap["Forward"]->addPass(pass);
	}

	void PipelineManager::loadAllShaderToRenderer(BaseGraphics* graphics)
	{
		// 		auto pipelineGroup = graphics->currentPipeline();
		// 
		// 		for (auto pass : mPassList)
		// 		{
		// 			pipelineGroup->addPass(pass);
		// 		}
	}

	void PipelineManager::addRenderObject(uint32_t layerIndex, IRenderObject* renderObject)
	{
		mRenderLayerList[layerIndex]->addRenderObejct(renderObject);
	}

	void PipelineManager::addLight(uint32_t layerIndex, ILight* light)
	{
		mLightLayerList[layerIndex]->addLight(light);
	}

	void PipelineManager::add(const std::string& name, Pipeline* pl)
	{
		sPipelineMap.emplace(name, pl);
	}

	Pipeline* PipelineManager::get(const std::string& name)
	{
		auto pl = sPipelineMap.find(name);
		if (pl != sPipelineMap.end())
		{
			return pl->second;
		}

		return nullptr;
	}
}
