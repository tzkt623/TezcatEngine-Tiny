#include "BaseGraphics.h"
#include "RenderLayer.h"

#include "../Manager/CameraManager.h"
#include "../Pipeline/Pipeline.h"
#include "../Component/Camera.h"
#include "../Component/RenderObject.h"
#include "../Scene/GameObject.h"
#include "../Component/MeshRenderer.h"
#include "../Statistic.h"

namespace tezcat::Tiny::Core
{
	BaseGraphics::BaseGraphics()
	{
		m_RenderLayerList.resize(32, new RenderLayer());
		CameraMgr::getInstance()->eventCameraAdded.addListener(this, BindEvent_1(BaseGraphics::onCameraAdd));
	}

	BaseGraphics::~BaseGraphics()
	{

	}

	void BaseGraphics::render()
	{
		TINY_PROFILER_TIMER_OUT(Statistic::RenderTime);
		this->preRender();
		this->onRender();
		this->postRender();
	}

	void BaseGraphics::preRender()
	{
		Statistic::DrawCall = 0;
		Statistic::PassCount = 0;
	}

	void BaseGraphics::onRender()
	{
		auto& cameras = CameraMgr::getInstance()->getAllCamera();
		for (auto camera : cameras)
		{
			this->updateViewport(camera);
			m_PipelineWithCamera[camera->getID()]->render(camera);
		}
	}

	void BaseGraphics::postRender()
	{

	}

	void BaseGraphics::cullWithCamera(Camera* camera)
	{
		for (auto& index : camera->getCullLayerList())
		{
			m_RenderLayerList[index]->cullGameObject(camera);
		}
	}

	void BaseGraphics::addRenderObject(IRenderObejct* renderObject)
	{
		auto go = dynamic_cast<Component*>(renderObject)->getGameObject();
		m_RenderLayerList[go->getLayerMaskIndex()]->addRenderObejct(renderObject);
	}

	void BaseGraphics::sendMeshData(uint32_t index, MeshRenderer* meshRenderer)
	{
		m_PipelineWithCamera[index]->addMeshRenderer(meshRenderer);
	}

	void BaseGraphics::onCameraAdd(Camera* camera)
	{
		while (m_PipelineWithCamera.size() <= camera->getID())
		{
			m_PipelineWithCamera.emplace_back(new PipelineGroup())->init();
		}
	}
}

