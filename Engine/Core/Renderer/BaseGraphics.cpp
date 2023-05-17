#include "BaseGraphics.h"
#include "RenderObject.h"
#include "ShadowRenderer.h"
#include "RenderHelper.h"

#include "../Manager/CameraManager.h"
#include "../Manager/LightManager.h"
#include "../Manager/FrameBufferManager.h"
#include "../Manager/ShadowCasterManager.h"

#include "../Renderer/RenderLayer.h"
#include "../Renderer/EnvironmentLighting.h"
#include "../Renderer/RenderPass.h"

#include "../Component/Camera.h"
#include "../Component/GameObject.h"
#include "../Component/MeshRenderer.h"
#include "../Component/Light.h"
#include "../Component/ShadowCaster.h"

#include "../Data/MeshData.h"

#include "../Profiler.h"



namespace tezcat::Tiny
{
	BaseGraphics::BaseGraphics()
		: mShadowCasterManager(new ShadowCasterManager())
		, mEnvLighting(new EnvironmentLighting())
		, mCameraManager(new CameraManager())
		, mLightManager(new LightManager())

	{
		RenderLayer::init();
	}

	BaseGraphics::~BaseGraphics()
	{
		delete mShadowCasterManager;
		delete mEnvLighting;
		delete mCameraManager;
		delete mLightManager;
	}


	void BaseGraphics::init(Engine* engine)
	{
		mEnvLighting->init();
	}

	void BaseGraphics::setPipeline(RenderPhase type, const std::string& name, Pipeline* pl)
	{
		while (mPipelineAry.size() < (int)type)
		{
			mPipelineAry.push_back(nullptr);
		}
		mPipelineAry[(int)type] = pl;
		mPipelineUMap.try_emplace(name, (int)type);
	}

	void BaseGraphics::render()
	{
		TINY_PROFILER_TIMER_OUT(Profiler::RenderTime);
		this->preRender();
		this->onRender();
		this->postRender();
	}

	void BaseGraphics::preRender()
	{
		Profiler_ResetDrawCall();
		Profiler_ResetPassCount();

		mShadowCasterManager->calculate(this);
		mCameraManager->calculate(this);
		mEnvLighting->calculate(this);
	}

	void BaseGraphics::onRender()
	{
		for (auto queue : mPreQueue)
		{
			queue->render(this);
		}

		for (auto queue : mBaseQueue)
		{
			queue->render(this);
		}

		for (auto queue : mPostQueue)
		{
			queue->render(this);
		}
	}

	void BaseGraphics::postRender()
	{
		mPreQueue.clear();
		mBaseQueue.clear();
		mPostQueue.clear();
	}


	//---------------------------------------------------------
	//
	//	draw
	//
	void BaseGraphics::drawLine(const glm::vec3& begin, const glm::vec3& end, const glm::vec3& color)
	{

	}

}
