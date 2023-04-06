#include "BaseGraphics.h"
#include "RenderObject.h"


#include "../Layer/RenderLayer.h"
#include "../Manager/CameraManager.h"
#include "../Pipeline/Pipeline.h"
#include "../Component/Camera.h"
#include "../Component/GameObject.h"
#include "../Component/MeshRenderer.h"
#include "../Statistic.h"



namespace tezcat::Tiny::Core
{
	BaseGraphics::BaseGraphics()
	{

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
		//#BaseGraphicsRender
		auto& cameras = CameraMgr::getInstance()->getSortedCameraAry();
		for (auto camera : cameras)
		{
			if (camera->isEnable())
			{
				camera->render(this);
			}
		}
	}

	void BaseGraphics::postRender()
	{

	}
}
