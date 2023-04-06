#include "Forward.h"
#include "Pipeline.h"
#include "PipelineQueue.h"
#include "../Renderer/RenderPass.h"

#include "../Manager/CameraManager.h"
#include "../Manager/PipelineManager.h"
#include "../Renderer/BaseGraphics.h"
#include "../Shader/Shader.h"
#include "../Shader/ShaderPackage.h"
#include "../Data/Material.h"
#include "../Component/MeshRenderer.h"
#include "../Component/Camera.h"
#include "../Manager/LightManager.h"
#include "../Layer/RenderLayer.h"
#include "../Component/Light.h"

namespace tezcat::Tiny::Core
{
	Forward::Forward()
		: mBackground(nullptr)
		, mGeometry(nullptr)
		, mAlphaTest(nullptr)
		, mTransparent(nullptr)
		, mOverlay(nullptr)
	{
		this->init();
	}

	Forward::~Forward()
	{
		delete mBackground;
		delete mGeometry;
		delete mAlphaTest;
		delete mTransparent;
		delete mOverlay;
	}

	void Forward::init()
	{
		mBackground = new PipelineQueue(PipelineQueue::Background, 1000);
		mGeometry = new PipelineQueue(PipelineQueue::Geometry, 2000);
		mAlphaTest = new PipelineQueue(PipelineQueue::AlphaTest, 3000);
		mTransparent = new PipelineQueue(PipelineQueue::Transparent, 5000);
		mOverlay = new PipelineQueue(PipelineQueue::Overlay, 6000);
	}

	void Forward::render(Camera* camera)
	{
		//#CameraCulling
		auto& cull_list = camera->getCullLayerList();
		for (auto index : cull_list)
		{
			PipelineMgr::getInstance()->getRenderLayer(index)->testWithCamera(camera);
		}

		//#LightCulling
// 		auto &points = LightMgr::getInstance()->getPointLights();
// 		for (auto point : points)
// 		{
// 			auto& list = point->getCullLayerList();
// 			for (auto index : list)
// 			{
// 				PipelineManager::getRenderLayer(index)->testVisibleObjects(point);
// 			}
// 		}


		mBackground->render(camera);

		mGeometry->render(camera);
		mAlphaTest->render(camera);
		//#解决覆盖问题
		mTransparent->render(camera);

		mOverlay->render(camera);
	}

	void Forward::addPass(RenderPass* pass)
	{
		auto shader = pass->getShader();
		switch (shader->getRenderQueue())
		{
		case PipelineQueue::Background:
			mBackground->addPass(pass);
			break;
		case PipelineQueue::Geometry:
			mGeometry->addPass(pass);
			break;
		case PipelineQueue::AlphaTest:
			mAlphaTest->addPass(pass);
			break;
		case PipelineQueue::Transparent:
			mTransparent->addPass(pass);
			break;
		case PipelineQueue::Overlay:
			mOverlay->addPass(pass);
			break;
		default:
			break;
		}
	}
}
