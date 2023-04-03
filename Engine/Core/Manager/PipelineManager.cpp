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
	std::vector<RenderPass*> PipelineManager::mPassList(100);
	std::vector<RenderLayer*> PipelineManager::mRenderLayerList(32, new RenderLayer());
	std::vector<LightLayer*> PipelineManager::mLightLayerList(32, new LightLayer());
	std::unordered_map<std::string, Pipeline*> PipelineManager::sPipelineMap =
	{
		{"Forward", new Forward()}
	};


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

	void PipelineManager::addRenderObject(uint32_t layerIndex, IRenderObejct* renderObject)
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
