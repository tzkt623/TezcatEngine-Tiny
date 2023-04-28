#include "BaseGraphics.h"
#include "RenderObject.h"
#include "ShadowRenderer.h"
#include "RenderHelper.h"

#include "../Manager/CameraManager.h"
#include "../Manager/LightManager.h"
#include "../Manager/PipelineManager.h"
#include "../Manager/FrameBufferManager.h"

#include "../Renderer/RenderLayer.h"
#include "../Pipeline/Pipeline.h"

#include "../Component/Camera.h"
#include "../Component/GameObject.h"
#include "../Component/MeshRenderer.h"
#include "../Component/Light.h"

#include "../Data/MeshData.h"

#include "../Statistic.h"



namespace tezcat::Tiny
{
	BaseGraphics::BaseGraphics()
	{
		RenderLayer::init();
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
		RenderHelper::renderPreData();
	}

	void BaseGraphics::onRender()
	{
		//#PipelineManager::render
		auto& data = CameraMgr::getInstance()->getCameraData();
		if (data.lock())
		{
			auto cameras = data->getAllCamera();
			for (auto camera : cameras)
			{
				if (camera->isEnable())
				{
					camera->render(this);
				}
			}
		}
	}

	void BaseGraphics::postRender()
	{
		//RenderCommander::postRender();
	}

	void BaseGraphics::setShadowMap(int width, int height)
	{
		ShadowRenderer::init(0, 0, width, height);
	}

	void BaseGraphics::drawLine(const glm::vec3& begin, const glm::vec3& end, const glm::vec3& color)
	{
		MeshData* meshdata = new MeshData();
		meshdata->vertices.emplace_back(begin);
		meshdata->vertices.emplace_back(end);
	}
}
