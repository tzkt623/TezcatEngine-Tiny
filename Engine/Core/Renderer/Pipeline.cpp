#include "Pipeline.h"
#include "BaseGraphics.h"
#include "RenderCommand.h"
#include "FrameBuffer.h"
#include "RenderObserver.h"
#include "RenderObjectCache.h"

#include "../Head/GLMHead.h"
#include "../Shader/Shader.h"

#include "../Component/Camera.h"
#include "../Component/GameObject.h"
#include "../Component/Transform.h"

#include "../Manager/CameraManager.h"
#include "../Manager/FrameBufferManager.h"
#include "../Manager/ShadowCasterManager.h"
#include "../Manager/LightingManager.h"

#include "../Profiler.h"
#include "../Event/EngineEvent.h"


namespace tezcat::Tiny
{
#define TINY_MASK_NONE_OF(value, mask) ((value & mask) == 0)

	Pipeline* PipelineHelper::sPipeline = nullptr;

	//-------------------------------------------
	//
	// PipelinePass
	//
	TINY_OBJECT_CPP(PipelinePass, TinyObject)

	PipelinePass::PipelinePass(uint32 globalFunctionMask)
		: mShader(nullptr)
		, mRenderObserver(nullptr)
		, mOrderID(0)
		, mFrameBuffer(nullptr)
		, mNeedRemoved(false)
		, mExited(true)
		, mMode(Mode::Continued)
	{
		this->resetGlobalFunction(globalFunctionMask);
	}

	PipelinePass::~PipelinePass()
	{
		mShader = nullptr;
		if (mFrameBuffer)
		{
			mFrameBuffer->deleteObject();
		}
	}

	uint32 PipelinePass::getPipelineOrderID() const
	{
		return ((mRenderObserver->getOrderID() + 127) << 24) | ((uint32)mShader->getRenderQueue() << 16) | mOrderID;
	}

	void PipelinePass::beginRender(BaseGraphics* graphics)
	{
		FrameBuffer* frame_buffer = nullptr;
		if (mFrameBuffer != nullptr)
		{
			frame_buffer = mFrameBuffer;
		}
		else if (mRenderObserver->getFrameBuffer())
		{
			frame_buffer = mRenderObserver->getFrameBuffer();
		}
		else
		{
			frame_buffer = FrameBufferManager::getDefaultBuffer();
		}

		FrameBufferManager::bind(graphics, frame_buffer);
		if (Pipeline::getFrameCount() != frame_buffer->currentFrame())
		{
			frame_buffer->updateCurrentFrame(Pipeline::getFrameCount());
			graphics->clear(mRenderObserver->getClearOption());
			graphics->setViewport(mRenderObserver->getViewportInfo());
		}
	}

	void PipelinePass::render(BaseGraphics* graphics)
	{
		mShader->resetGlobalState();
		graphics->bind(mShader);
		graphics->setPassState(mShader);
		mRenderObserver->submitViewMatrix(graphics, mShader);

		for (auto& fun : mGlobalSubmitArray)
		{
			fun(graphics, mShader);
		}

		for (auto cmd : mCommandArray)
		{
			cmd->run(graphics, mShader);
			delete cmd;
		}

		mCommandArray.clear();
	}

	void PipelinePass::endRender(BaseGraphics* graphics)
	{
		if (mFrameBuffer != nullptr)
		{
			FrameBufferManager::unbind(graphics, mFrameBuffer);
		}
		else if (mRenderObserver->getFrameBuffer())
		{
			FrameBufferManager::unbind(graphics, mRenderObserver->getFrameBuffer());
		}

		if (mMode == Mode::Once)
		{
			this->removeFromPipeline();
		}
	}

	void PipelinePass::addCommand(RenderCommand* cmd)
	{
		mCommandArray.push_back(cmd);
	}

	void PipelinePass::resetGlobalFunction(uint32 globalFunctionMask)
	{
		mGlobalSubmitArray.clear();

		if (TINY_MASK_NONE_OF(globalFunctionMask, GlobalSubmit::NO_SHADOW))
		{
			mGlobalSubmitArray.emplace_back(TINY_BIND2(ShadowCasterManager::submit));
		}

		if (TINY_MASK_NONE_OF(globalFunctionMask, GlobalSubmit::NO_LIGHTING))
		{
			mGlobalSubmitArray.emplace_back(TINY_BIND2(LightingManager::submitLighting));
		}

		if (TINY_MASK_NONE_OF(globalFunctionMask, GlobalSubmit::NO_ENVLIGHTING))
		{
			mGlobalSubmitArray.emplace_back(TINY_BIND2(LightingManager::submitEnvLighting));
		}
	}

	uint32 PipelinePass::getUID() const
	{
		return mShader->getUID();
	}

	void PipelinePass::addToPipeline()
	{
		if (mExited)
		{
			PipelineHelper::addPipePass(this);
		}
		else
		{
			//因为Pass的删除是在下一帧进行的
			//所以如果没有退出,并且是Once模式时
			//调用此函数说明这一帧需要保留此Pass
			if (mMode == Mode::Once)
			{
				mNeedRemoved = false;
				mExited = false;
			}
		}
	}

	void PipelinePass::removeFromPipeline()
	{
		//如果已经退出了
		//就不能被移除
		if (mExited)
		{
			return;
		}

		mNeedRemoved = true;
	}

	void PipelinePass::onClose()
	{
		mGlobalSubmitArray.clear();
	}

	void PipelinePass::setFrameBuffer(FrameBuffer* frameBuffer)
	{
		mFrameBuffer = frameBuffer;
		mFrameBuffer->saveObject();
	}

	void PipelinePass::onEnterPipeline()
	{
		mExited = false;
		mNeedRemoved = false;
	}

	void PipelinePass::onExitPipeline()
	{
		mExited = true;
		mNeedRemoved = false;
	}

#pragma region ObserverPipePass
	TINY_OBJECT_CPP(ObserverPipelinePass, PipelinePass)

	ObserverPipelinePass::ObserverPipelinePass(BaseRenderObserver* renderObserver
			, Shader* shader
			, uint32 globalFunctionMask)
		: PipelinePass(globalFunctionMask)

	{
		mRenderObserver = renderObserver;
		mShader = shader;
		mName = shader->getName();
	}

	ObserverPipelinePass::~ObserverPipelinePass()
	{
		mRenderObserver = nullptr;
		mShader = nullptr;
	}

#pragma endregion


#pragma region ReplacedPipePass
	TINY_OBJECT_CPP(ReplacedPipelinePass, PipelinePass)

	ReplacedPipelinePass::ReplacedPipelinePass(BaseRenderObserver* renderObserver
			, Shader* shader
			, uint32 globalFunctionMask)
		: PipelinePass(globalFunctionMask)
		, mUseCullLayer(false)
		, mPreFunction()
	{
		mRenderObserver = renderObserver;
		mRenderObserver->saveObject();

		mShader = shader;
		mName = shader->getName();
	}

	ReplacedPipelinePass::~ReplacedPipelinePass()
	{
		mRenderObserver->deleteObject();
		mRenderObserver = nullptr;
		mShader = nullptr;
	}

	void ReplacedPipelinePass::preCalculate(BaseGraphics* graphics)
	{
		if (mUseCullLayer)
		{
			//先剔除
			for (auto& index : mRenderObserver->getCullLayerList())
			{
				//剔除到对应的渲染通道
				RenderObjectCache::culling(index, graphics, this);
			}
		}

		if (mPreFunction)
		{
			mPreFunction(graphics, this);
		}
	}

#pragma endregion



#pragma region Pipeline
	//-------------------------------------------
	//
	//	Pipeline
	//
	uint32 Pipeline::sFrameCount = 0;

	Pipeline::Pipeline()
	{
		PipelineHelper::setPipeline(this);
	}

	void Pipeline::init()
	{
		RenderObjectCache::init();
		FrameBufferManager::init();
		LightingManager::init();
		ShadowCasterManager::init();
	}

	void Pipeline::render(BaseGraphics* graphics)
	{
		TINY_PROFILER_TIMER_OUT(Profiler::RenderTime);
		this->preRender(graphics);
		this->onRender(graphics);
		this->postRender(graphics);
	}

	//-------------------------------------------
	//
	//	PipelineBuildin
	//
	PipelineBuildin::PipelineBuildin()
		: mDirty(true)
	{
		EngineEvent::get()->addListener(EngineEventID::EE_OnPopScene, this
			, [this](const EventData& data)
			{
				for (auto pass : mReplacedPipePassArray)
				{
					pass->deleteObject();
				}
				mReplacedPipePassArray.clear();

				for (auto pass : mPassArray)
				{
					pass->onExitPipeline();
					pass->deleteObject();
				}
				mPassArray.clear();
			});
	}

	PipelineBuildin::~PipelineBuildin()
	{
		EngineEvent::get()->removeListener(this);
	}

	void PipelineBuildin::preRender(BaseGraphics* graphics)
	{
		Profiler_ResetDrawCall();
		Profiler_ResetPassCount();

		CameraManager::calculate(graphics);
		LightingManager::calculate();
	}

	void PipelineBuildin::onRender(BaseGraphics* graphics)
	{
		if (!mReplacedPipePassArray.empty())
		{
			auto it = mReplacedPipePassArray.begin();
			while (it != mReplacedPipePassArray.end())
			{
				auto pass = *it;
				if (pass->isExited())
				{
					pass->deleteObject();
					it = mReplacedPipePassArray.erase(it);
				}
				else
				{
					pass->preCalculate(graphics);
					it++;
				}
			}
		}

		//对queue进行排序
		if (mDirty)
		{
			mDirty = false;
			std::ranges::sort(mPassArray.begin(), mPassArray.end(), [](PipelinePass* a, PipelinePass* b)
			{
				return a->getPipelineOrderID() < b->getPipelineOrderID();
			});
		}

		auto it = mPassArray.begin();
		while (it != mPassArray.end())
		{
			auto pass = *it;
			if (pass->isNeedRemoved())
			{
				it = mPassArray.erase(it);
				pass->onExitPipeline();
				pass->deleteObject();
			}
			else
			{
				pass->beginRender(graphics);
				pass->render(graphics);
				pass->endRender(graphics);
				it++;
			}
		}
	}

	void PipelineBuildin::postRender(BaseGraphics* graphics)
	{
		sFrameCount++;
	}

	void PipelineBuildin::addPipePass(PipelinePass* pass)
	{
		mDirty = true;
		mPassArray.push_back(pass);
		pass->onEnterPipeline();
		pass->saveObject();

		if (pass->typeIDEqual<ReplacedPipelinePass>())
		{
			mReplacedPipePassArray.push_back((ReplacedPipelinePass*)pass);
			pass->saveObject();
		}
	}

#pragma endregion


}
