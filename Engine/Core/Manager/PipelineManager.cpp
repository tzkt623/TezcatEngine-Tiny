#include "PipelineManager.h"
#include "../Shader/Shader.h"

#include "../Renderer/RenderPass.h"
#include "../Renderer/BaseGraphics.h"
#include "../Renderer/RenderObject.h"

#include "../Layer/RenderLayer.h"
#include "../Layer/LightLayer.h"

#include "../Component/Light.h"
#include "../Component/Camera.h"
#include "../Pipeline/Forward.h"

#include "LightManager.h"
#include "CameraManager.h"


namespace tezcat::Tiny
{
	PipelineManager::PipelineManager()
	{
		SG<PipelineManager>::attach(this);
		this->add("Forward", new Forward());
	}

	PipelineManager::~PipelineManager()
	{

	}

	void PipelineManager::addLight(uint32_t layerIndex, ILight* light)
	{
		mLightLayerAry[layerIndex]->addLight(light);
	}


	uint32_t PipelineManager::add(const std::string& name, Pipeline* pl)
	{
		uint32_t index = (uint32_t)mPipelineAry.size();
		mPipelineAry.push_back(pl);
		mPipelineUMap.emplace(name, index);
		return index;
	}

	Pipeline* PipelineManager::get(const std::string& name)
	{
		auto pl = mPipelineUMap.find(name);
		if (pl != mPipelineUMap.end())
		{
			return mPipelineAry[pl->second];
		}

		return nullptr;
	}

	void PipelineManager::preRender(BaseGraphics* graphics)
	{

	}

	void PipelineManager::render(BaseGraphics* graphics)
	{

	}

	void PipelineManager::postRender(BaseGraphics* graphics)
	{

	}
}
