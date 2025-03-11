/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Core/Renderer/Pipeline.h"
#include "Core/Renderer/BaseGraphics.h"
#include "Core/Renderer/RenderCommand.h"
#include "Core/Renderer/FrameBuffer.h"
#include "Core/Renderer/RenderObserver.h"
#include "Core/Renderer/RenderObjectCache.h"
#include "Core/Renderer/Renderer.h"

#include "Core/Head/GLMHead.h"
#include "Core/Shader/Shader.h"

#include "Core/Component/Camera.h"
#include "Core/Component/GameObject.h"
#include "Core/Component/Transform.h"

#include "Core/Manager/CameraManager.h"
#include "Core/Manager/FrameBufferManager.h"
#include "Core/Manager/ShadowCasterManager.h"
#include "Core/Manager/LightingManager.h"
#include "Core/Manager/GameObjectManager.h"

#include "Core/Debug/Debug.h"

#include "Core/Event/EngineEvent.h"


namespace tezcat::Tiny
{
#define TINY_MASK_NONE_OF(value, mask) ((value & mask) == 0)

	Pipeline* PipelineHelper::sPipeline = nullptr;

	//-------------------------------------------
	//
	// PipelinePass
	//
	TINY_OBJECT_CPP(PipelinePass, TinyObject);

	PipelinePass::PipelinePass(uint32_t globalFunctionMask)
		: mShader(nullptr)
		, mRenderObserver(nullptr)
		, mPipelineOrderID(0)
		, mFrameBuffer(nullptr)
		, mNeedRemoved(false)
		, mIsInPipeline(false)
		, mMode(Mode::Continued)
		, mIsOnceModeExecuted(false)
	{
		this->resetGlobalFunction(globalFunctionMask);
	}

	PipelinePass::~PipelinePass()
	{
		if (mFrameBuffer)
		{
			mFrameBuffer->deleteObject();
		}

		mRenderObserver = nullptr;
		mShader = nullptr;
	}

	void PipelinePass::beginRender()
	{
		TINY_PROFILER_PASSCOUNT(1);
		Graphics::getInstance()->buildCommand();

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

		FrameBufferManager::bind(frame_buffer);
		if (Pipeline::getFrameCount() != frame_buffer->currentFrame())
		{
			frame_buffer->updateCurrentFrame(Pipeline::getFrameCount());
			Graphics::getInstance()->clear(mRenderObserver->getClearOption());
		}

		Graphics::getInstance()->setViewport(mRenderObserver->getViewportInfo());
	}

	void PipelinePass::render()
	{
		mShader->resetGlobalState();
		Graphics::getInstance()->bind(mShader);
		Graphics::getInstance()->setPassState(mShader);
		mRenderObserver->submit(mShader);

		for (auto& fun : mGlobalSubmitArray)
		{
			fun(mShader);
		}

		for (auto cmd : mCommandArray)
		{
			cmd->execute(this, mShader);
			delete cmd;
		}

		mCommandArray.clear();
	}

	void PipelinePass::endRender()
	{
		if (mFrameBuffer != nullptr)
		{
			FrameBufferManager::unbind(mFrameBuffer);
		}
		else if (mRenderObserver->getFrameBuffer())
		{
			FrameBufferManager::unbind(mRenderObserver->getFrameBuffer());
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

	void PipelinePass::resetGlobalFunction(uint32_t globalFunctionMask)
	{
		mGlobalSubmitArray.clear();

		if (TINY_MASK_NONE_OF(globalFunctionMask, GlobalSubmit::NO_SHADOW))
		{
			mGlobalSubmitArray.emplace_back(TINY_BIND(ShadowCasterManager::submit));
		}

		if (TINY_MASK_NONE_OF(globalFunctionMask, GlobalSubmit::NO_LIGHTING))
		{
			mGlobalSubmitArray.emplace_back(TINY_BIND(LightingManager::submitLighting));
		}

		if (TINY_MASK_NONE_OF(globalFunctionMask, GlobalSubmit::NO_ENVLIGHTING))
		{
			mGlobalSubmitArray.emplace_back(TINY_BIND(LightingManager::submitEnvLighting));
		}
	}

	uint32_t PipelinePass::getUID() const
	{
		return mShader->getUID();
	}

	void PipelinePass::addToPipeline()
	{
		//如果某些系统每帧都要计算管线是否需要生效
		//就需要保护好管线状态
		switch (mMode)
		{
		case Mode::Continued:
			//如果已经在管线中了,就不需要添加
			if (mIsInPipeline)
			{
				return;
			}

			PipelineHelper::addPipePass(this);
			break;
		case Mode::Once:
			if (!mIsOnceModeExecuted)
			{
				mIsOnceModeExecuted = true;
				PipelineHelper::addPipePass(this);
			}
			break;
		default:
			break;
		}
	}

	void PipelinePass::removeFromPipeline()
	{
		//如果已经退出了
		//就不能被移除
		if (!mIsInPipeline)
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
		//if (mFrameBuffer)
		//{
		//	mFrameBuffer->deleteObject();
		//}
		//mFrameBuffer = frameBuffer;
		//mFrameBuffer->saveObject();
		//
		//mFrameBuffer = TinyObject::setObject(mFrameBuffer, frameBuffer);

		TINY_OBJECT_SET(mFrameBuffer, frameBuffer);
	}

	void PipelinePass::onEnterPipeline()
	{
		mIsInPipeline = true;
		mNeedRemoved = false;
	}

	void PipelinePass::onExitPipeline()
	{
		mIsInPipeline = false;
		mNeedRemoved = false;
	}

	void PipelinePass::setObserver(BaseRenderObserver* observer)
	{
		//if (mRenderObserver)
		//{
		//	mRenderObserver->deleteObject();
		//}
		//mRenderObserver = observer;
		//mRenderObserver->saveObject();

		mRenderObserver = observer;
		mType2.observerOrder = (uint8_t)(mRenderObserver->getOrderID() + 127);
	}

	void PipelinePass::setShader(Shader* shader)
	{
		mShader = shader;
		mType2.shaderQueueID = (uint8_t)mShader->getRenderQueue();
	}

	std::string PipelinePass::getMemoryInfo()
	{
		return TINY_OBJECT_MEMORY_INFO();
	}

#pragma region ObserverPipePass
	TINY_OBJECT_CPP(ObserverPipelinePass, PipelinePass);

	ObserverPipelinePass::ObserverPipelinePass(BaseRenderObserver* renderObserver
			, Shader* shader
			, uint32_t globalFunctionMask)
		: PipelinePass(globalFunctionMask)

	{
		this->setObserver(renderObserver);

		this->setShader(shader);
		mName = shader->getName();
		mEngineName = mName;
	}

	ObserverPipelinePass::~ObserverPipelinePass()
	{
		//mRenderObserver->deleteObject();
	}

#pragma endregion


#pragma region ReplacedPipePass
	TINY_OBJECT_CPP(ReplacedPipelinePass, PipelinePass);

	ReplacedPipelinePass::ReplacedPipelinePass(BaseRenderObserver* renderObserver
			, Shader* shader
			, uint32_t globalFunctionMask)
		: PipelinePass(globalFunctionMask)
		, mCustomCulling()
		, mCreateFunction()
	{
		this->setObserver(renderObserver);

		this->setShader(shader);
		mName = shader->getName();
		mEngineName = mName;

		mCreateFunction = TINY_BIND_THIS(ReplacedPipelinePass::createCommand);
	}

	ReplacedPipelinePass::ReplacedPipelinePass(Shader* shader
		, uint32_t globalFunctionMask /*= GLOBAL_NONE*/)
		: PipelinePass(globalFunctionMask)
		, mCustomCulling()
		, mCreateFunction()
	{
		this->setShader(shader);
		mName = shader->getName();
		mEngineName = mName;

		mCreateFunction = TINY_BIND_THIS(ReplacedPipelinePass::createCommand);
	}

	ReplacedPipelinePass::~ReplacedPipelinePass()
	{
		//if (mRenderObserver)
		//{
		//	mRenderObserver->deleteObject();
		//}
		mCreateFunction = nullptr;
		mCustomCulling = nullptr;
	}

	void ReplacedPipelinePass::preCalculate()
	{
		if (!mRenderObserver)
		{
			throw std::bad_exception();
			//mRenderObserver = CameraManager::getMainCamera()->getRenderObserver();
			//mRenderObserver->saveObject();
		}

		mRenderObserver->prepareRender();

		if (!mCustomCulling)
		{
			//先剔除
			for (auto& index : mRenderObserver->getCullLayerList())
			{
				//剔除到对应的渲染通道
				RenderObjectCache::culling(index, this);
			}
		}
		else
		{
			mCustomCulling(this);
		}
	}

	void ReplacedPipelinePass::pushCommand(BaseMeshRenderer* meshRenderer)
	{
		this->addCommand(mCreateFunction(meshRenderer));
	}

	RenderCommand* ReplacedPipelinePass::createCommand(BaseMeshRenderer* renderer)
	{
		return new RenderCMD_DrawMeshWithOutMaterial(renderer->getVertex(), renderer->getTransform());
	}

#pragma endregion



#pragma region Pipeline
	//-------------------------------------------
	//
	//	Pipeline
	//
	uint32_t Pipeline::sFrameCount = 0;

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
		GameObjectManager::init();
	}

	void Pipeline::render()
	{
		TINY_PROFILER_TIMER_OUT(Profiler::RenderTime);
		this->preRender();
		this->onRender();
		this->postRender();
	}

	//-------------------------------------------
	//
	//	PipelineBuildin
	//
	PipelineBuildin::PipelineBuildin()
		: mDirty(true)
	{
		EngineEvent::getInstance()->addListener(EngineEventID::EE_OnPopScene, this
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
		EngineEvent::getInstance()->removeListener(this);
	}

	void PipelineBuildin::preRender()
	{
		TINY_PROFILER_RESET_DRAWCALL();
		TINY_PROFILER_RESET_PASSCOUNT();

		Graphics::getInstance()->buildCommand();

		CameraManager::calculate();
		LightingManager::calculate();
		GameObjectManager::calculate();

		//自定义pass预计算
		//这个结构可能要优化
		if (!mReplacedPipePassArray.empty())
		{
			auto it = mReplacedPipePassArray.begin();
			auto end = mReplacedPipePassArray.end();
			while (it != end)
			{
				auto pass = *it;
				if (pass->isNeedRemoved())
				{
					it = mReplacedPipePassArray.erase(it);
					end = mReplacedPipePassArray.end();

					pass->deleteObject();
				}
				else
				{
					pass->preCalculate();
					it++;
				}
			}
		}

		Graphics::getInstance()->buildCommand();

		//对queue进行排序
		if (mDirty)
		{
			mDirty = false;
			std::ranges::sort(mPassArray.begin(), mPassArray.end(), [](PipelinePass* a, PipelinePass* b)
			{
				return a->getPipelineOrderID() < b->getPipelineOrderID();
			});
		}
	}

	void PipelineBuildin::onRender()
	{
		auto it = mPassArray.begin();
		auto end = mPassArray.end();
		while (it != end)
		{
			auto pass = *it;
			if (pass->isNeedRemoved())
			{
				it = mPassArray.erase(it);
				end = mPassArray.end();

				pass->onExitPipeline();
				pass->deleteObject();
			}
			else
			{
				pass->beginRender();
				pass->render();
				pass->endRender();
				it++;
			}
		}
	}

	void PipelineBuildin::postRender()
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
