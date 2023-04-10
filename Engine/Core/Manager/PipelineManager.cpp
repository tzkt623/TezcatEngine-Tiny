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


namespace tezcat::Tiny::Core
{
	PipelineManager::PipelineManager()
	{
		SG<PipelineManager>::attach(this);

		mRenderPassAry.reserve(64);
		mRenderLayerAry.reserve(32);
		for (int i = 0; i < 32; i++)
		{
			mRenderLayerAry.push_back(new RenderLayer());
		}

		this->add("Forward", new Forward());
	}

	PipelineManager::~PipelineManager()
	{

	}

	void PipelineManager::createPass(Shader* shader)
	{
		while (mRenderPassAry.size() <= shader->getUID())
		{
			mRenderPassAry.push_back(nullptr);
		}

		auto pass = new RenderPass(shader);
		mRenderPassAry[shader->getUID()] = pass;
		mPipelineUMap["Forward"]->addPass(pass);
	}

	void PipelineManager::addRenderObject(uint32_t layerIndex, IRenderObject* renderObject)
	{
		mRenderLayerAry[layerIndex]->addRenderObejct(renderObject);
	}

	void PipelineManager::addLight(uint32_t layerIndex, ILight* light)
	{
		mLightLayerAry[layerIndex]->addLight(light);
	}


	void PipelineManager::addCastShadowObject(IRenderMesh* renderMesh)
	{

	}


	uint32_t PipelineManager::add(const std::string& name, Pipeline* pl)
	{
		uint32_t index = mPipelineAry.size();
		mPipelineAry.push_back(pl);
		mPipelineUMap.emplace(name, index);
		return index;
	}

	Pipeline* PipelineManager::get(const std::string& name)
	{
		auto pl = mPipelineUMap.find(name);
		if (pl != mPipelineUMap.end())
		{
			return pl->second;
		}

		return nullptr;
	}


	//#有问题
	void PipelineManager::setShadowPass(bool value)
	{
		if (value)
		{
			auto it = mPipelineUMap.find("ShadowMap");
			if (it != mPipelineUMap.end())
			{

			}
		}
		else
		{
			mShadowPass = nullptr;
		}
	}
	void PipelineManager::preRender(BaseGraphics* graphics)
	{
		//render shadow map
		if (mShadowPass != nullptr)
		{
			auto dir_light = LightMgr::getInstance()->getDirectionalLight();
			dir_light->render(graphics);
			mShadowPass->render(graphics, dir_light);
		}
	}

	void PipelineManager::render(BaseGraphics* graphics)
	{
		//#PipelineManager::render
		auto& cameras = CameraMgr::getInstance()->getSortedCameraAry();
		for (auto camera : cameras)
		{
			if (camera->isEnable())
			{
				camera->render(graphics);
			}
		}
	}

	void PipelineManager::postRender(BaseGraphics* graphics)
	{

	}
}
