#include "BaseGraphics.h"
#include "RenderObject.h"


#include "../Manager/CameraManager.h"
#include "../Manager/LightManager.h"
#include "../Manager/PipelineManager.h"
#include "../Manager/FrameBufferManager.h"

#include "../Layer/RenderLayer.h"
#include "../Pipeline/Pipeline.h"

#include "../Component/Camera.h"
#include "../Component/GameObject.h"
#include "../Component/MeshRenderer.h"
#include "../Component/Light.h"

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

		PipelineMgr::getInstance()->preRender(this);
	}

	void BaseGraphics::onRender()
	{
		//#BaseGraphics::onRender
		PipelineMgr::getInstance()->render(this);
	}

	void BaseGraphics::postRender()
	{
		PipelineMgr::getInstance()->postRender(this);
	}

	void BaseGraphics::createShadowRenderer(const std::string& texName, int width, int height)
	{
		FrameBufferMgr::getInstance()->createShadowMap(texName, width, height);
		PipelineMgr::getInstance()->setShadowPass(true);
	}

	bool BaseGraphics::bindShadowMap()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	bool BaseGraphics::isEnableShadow()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

}
