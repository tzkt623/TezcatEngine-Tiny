#include "Forward.h"
#include "Pipeline.h"
#include "PipelineQueue.h"
#include "../Renderer/RenderPass.h"
#include "../Renderer/BaseGraphics.h"
#include "../Renderer/Texture.h"
#include "../Renderer/ShadowRenderer.h"
#include "../Renderer/RenderLayer.h"
#include "../Renderer/FrameBuffer.h"

#include "../Manager/CameraManager.h"
#include "../Manager/PipelineManager.h"
#include "../Manager/LightManager.h"
#include "../Manager/FrameBufferManager.h"

#include "../Shader/Shader.h"
#include "../Shader/ShaderPackage.h"

#include "../Component/MeshRenderer.h"
#include "../Component/Camera.h"
#include "../Component/Light.h"

#include "../Data/Material.h"


namespace tezcat::Tiny
{
	Forward::Forward()
	{

	}

	Forward::~Forward()
	{

	}


	void Forward::render(BaseGraphics* graphics, Camera* camera)
	{
		//正常渲染
		auto& cull_list = camera->getCullLayerList();
		for (auto index : cull_list)
		{
			//剔除到渲染通道
			RenderLayer::getRenderLayer(index)->culling(camera, RenderPassType::Default);
		}

		//相机开始渲染
		camera->beginRender();
		graphics->setViewport(camera->getViewportInfo());
		graphics->clear(camera->getClearOption());

		sBackground->render(graphics, camera);

		auto dir_light = LightMgr::getInstance()->getDirectionalLight();
		auto& point_lights = LightMgr::getInstance()->getPointLights();

		sGeometry->render(graphics, camera, dir_light);
		for (auto p : point_lights)
		{
			sGeometry->render(graphics, camera, p);
		}

		sAlphaTest->render(graphics, camera, dir_light);
		for (auto p : point_lights)
		{
			sAlphaTest->render(graphics, camera, p);
		}

		sOpaqueLast->render(graphics, camera, dir_light);
		for (auto p : point_lights)
		{
			sOpaqueLast->render(graphics, camera, p);
		}

		sTransparent->render(graphics, camera, dir_light);
		for (auto p : point_lights)
		{
			//#解决覆盖问题
			sTransparent->render(graphics, camera, p);
		}

		sOverlay->render(graphics, camera);
		camera->endRender();
	}

	void Forward::render(BaseGraphics* graphics, IRenderObserver* renderObject)
	{

	}

}
