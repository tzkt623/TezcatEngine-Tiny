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

	void Forward::render(BaseGraphics* graphics, Camera* camera)
	{
		//#CameraCulling
		auto& cull_list = camera->getCullLayerList();
		for (auto index : cull_list)
		{
			PipelineMgr::getInstance()
				->getRenderLayer(index)
				->testWithCamera(camera);
		}

		//Shadow和Camera一样都是世界的观察者
		//所以renderpass如何确定是哪个观察者进来观察
		//如果是灯光进来观察,说明是渲染阴影,那么只需要object提供顶点数据
		//那就是说要有一个observer列表,用于对当前的所有observer进行一个排序
		//然后按照顺序依次进来渲染
		if (graphics->isEnableShadow())
		{
			auto dir_light = LightMgr::getInstance()->getDirectionalLight();
			mShadowMap->render(graphics, dir_light);
		}
		
		
		mBackground->render(graphics, camera);

		mGeometry->render(graphics, camera);
		mAlphaTest->render(graphics, camera);
		//#解决覆盖问题
		mTransparent->render(graphics, camera);

		mOverlay->render(graphics, camera);
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
