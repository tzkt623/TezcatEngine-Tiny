/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

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
#include "Core/Renderer/PipelineWorker.h"
#include "Core/Renderer/Pipeline.h"
#include "Core/Renderer/BaseGraphics.h"
#include "Core/Renderer/FrameBuffer.h"
#include "Core/Renderer/RenderObjectCache.h"
#include "Core/Renderer/RenderCommand.h"
#include "Core/Renderer/Renderer.h"

#include "Core/Manager/PipelineManager.h"
#include "Core/Manager/FrameBufferManager.h"
#include "Core/Manager/ShadowCasterManager.h"
#include "Core/Manager/LightingManager.h"

#include "Core/Shader/Shader.h"

#include "Core/Debug/Profiler.h"


namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(PipelineWorker, TinyObject);

	PipelineWorker::PipelineWorker()
		: mRemoveFlag(false)
		, mIsInPipeline(false)
	{

	}

	void PipelineWorker::onEnterPipeline()
	{
		mIsInPipeline = true;
		mRemoveFlag = false;
	}

	void PipelineWorker::onExitPipeline()
	{
		mIsInPipeline = false;
		mRemoveFlag = false;
	}

	void PipelineWorker::removeFromPipeline()
	{
		//如果已经退出了
		//就不能被移除
		if (!mIsInPipeline)
		{
			return;
		}

		mRemoveFlag = true;
	}

	void PipelineWorker::setObserver(BaseRenderObserver* observer)
	{
		mRenderObserver.reset(observer);
	}

	//-------------------------------------------
	//
	// PipelinePass
	//
	TINY_OBJECT_CPP(PipelinePass, TinyObject);

	PipelinePass::PipelinePass(uint32_t globalFunctionMask)
		: mPipelineSortingID(0)
		, mShader(nullptr)
		, mFrameBuffer(nullptr)
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
		mQueue = nullptr;
	}

	void PipelinePass::beginRender()
	{
		TINY_PROFILER_PASSCOUNT(1);
		TINY_PIPELINE_INFO_PUSH(std::format("Shader:{}", mShader->getName()));

		Graphics::getInstance()->buildCommand();
		mFrameBufferChanged = false;
		if (mFrameBuffer != nullptr)
		{
			if (Pipeline::getFrameCount() != mFrameBuffer->currentFrame())
			{
				mFrameBufferChanged = true;
				mFrameBuffer->updateCurrentFrame(Pipeline::getFrameCount());
				Graphics::getInstance()->bind(mFrameBuffer);

				Graphics::getInstance()->setClearColor(mRenderObserver->getClearColor());
				Graphics::getInstance()->clear(mRenderObserver->getClearOption());

				Graphics::getInstance()->setViewport(mRenderObserver->getViewRect());
			}
		}
	}

	void PipelinePass::onRender()
	{
		mShader->resetGlobalState();
		Graphics::getInstance()->bind(mShader);
		Graphics::getInstance()->setPassState(mShader);
		mRenderObserver->submit(mShader);
		//GlobalSlotManager::submit(mShader);


		switch (mShader->getLightMode())
		{
		case LightMode::Forward:
			break;
		case LightMode::Deferred:
			break;
		case LightMode::ForwardAdd:
			break;
		default:
			break;
		}

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
		if (mFrameBufferChanged)
		{
			Graphics::getInstance()->unbind(mFrameBuffer);
		}

		if (mMode == Mode::Once)
		{
			this->removeFromPipeline();
		}

		TINY_PIPELINE_INFO_POP();
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
		
			mQueue->addPipelinePass(this);
			break;
		case Mode::Once:
			if (!mIsInPipeline && !mIsOnceModeExecuted)
			{
				mIsOnceModeExecuted = true;
				mQueue->addPipelinePass(this);
			}
			break;
		default:
			break;
		}
	}

	void PipelinePass::onClose()
	{
		mGlobalSubmitArray.clear();
		for (auto cmd : mCommandArray)
		{
			delete cmd;
		}
		mCommandArray.clear();
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

	void PipelinePass::setShader(Shader* shader)
	{
		mShader = shader;
		mSortingIDType2.shader = (uint16_t)mShader->getRenderQueue() + shader->getSortingOffset();
	}

	std::string PipelinePass::getMemoryInfo()
	{
		return TINY_OBJECT_MEMORY_INFO();
	}

	void PipelinePass::render(BaseRenderObserver* observer)
	{
		mRenderObserver.reset(observer);
		this->beginRender();
		this->onRender();
		this->endRender();
	}

	void PipelinePass::setObserver(BaseRenderObserver* observer)
	{
		Base::setObserver(observer);
		mQueue = observer->getPipelineQueue();
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

	}

#pragma endregion


#pragma region ReplacedPipePass
	std::vector<ReplacedPipelinePass*> ReplacedPipelinePass::mPreRenderPassArray;
	TINY_OBJECT_CPP(ReplacedPipelinePass, PipelinePass);

	ReplacedPipelinePass::ReplacedPipelinePass(BaseRenderObserver* renderObserver
			, Shader* shader
			, uint32_t globalFunctionMask)
		: PipelinePass(globalFunctionMask)
		, mIsAutoCulling(true)
	{
		this->setObserver(renderObserver);

		this->setShader(shader);
		mName = shader->getName();
		mEngineName = mName;

		mAutoGenerateCommand = TINY_BIND_THIS(ReplacedPipelinePass::createCommand);
	}

	ReplacedPipelinePass::~ReplacedPipelinePass()
	{
		mAutoGenerateCommand = nullptr;
		mCustomCulling = nullptr;
	}

	void ReplacedPipelinePass::preCalculate()
	{
		if (!mRenderObserver.lock())
		{
			return;
		}

		mRenderObserver->preRender();

		if (mIsAutoCulling)
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

		mQueue->addToPipeline();
	}

	void ReplacedPipelinePass::pushCommand(BaseMeshRenderer* meshRenderer)
	{
		this->addCommand(mAutoGenerateCommand(meshRenderer));
	}

	RenderCommand* ReplacedPipelinePass::createCommand(BaseMeshRenderer* renderer)
	{
		return new RenderCMD_DrawMeshWithOutMaterial(renderer->getVertex(), renderer->getTransform());
	}

	void ReplacedPipelinePass::preRender()
	{
		//自定义pass预计算
		//这个结构可能要优化
		if (!mPreRenderPassArray.empty())
		{
			auto it = mPreRenderPassArray.begin();
			auto end = mPreRenderPassArray.end();
			while (it != end)
			{
				auto pass = *it;
				if (pass->isNeedRemoved())
				{
					it = mPreRenderPassArray.erase(it);
					end = mPreRenderPassArray.end();

					pass->deleteObject();
				}
				else
				{
					pass->preCalculate();
					it++;
				}
			}
		}
	}

	void ReplacedPipelinePass::clearPassArray()
	{
		for (auto pass : mPreRenderPassArray)
		{
			pass->onExitPipeline();
			pass->deleteObject();
		}
		mPreRenderPassArray.clear();
	}

	void ReplacedPipelinePass::addToPipeline()
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

			mQueue->addPipelinePass(this);
			ReplacedPipelinePass::addPreRenderArray(this);
			break;
		case Mode::Once:
			if (!mIsInPipeline && !mIsOnceModeExecuted)
			{
				mIsOnceModeExecuted = true;
				mQueue->addPipelinePass(this);
				ReplacedPipelinePass::addPreRenderArray(this);
			}
			break;
		default:
			break;
		}
	}

#pragma endregion

#pragma region PipelineQueue
	TINY_OBJECT_CPP(PipelineQueue, TinyObject);
	PipelineQueue::PipelineQueue()
		: mDirty(true)
		, mSortingID(0)
		, mCountDown(0)
	{

	}

	PipelineQueue::~PipelineQueue()
	{

	}

	void PipelineQueue::addPipelinePass(PipelinePass* pass)
	{
		mDirty = true;
		pass->onEnterPipeline();
		pass->saveObject();
		mRenderingPassArray.push_back(pass);
	}

	void PipelineQueue::render()
	{
		auto observer = mRenderObserver.lock();
		if (!observer)
		{
			return;
		}

		TINY_PIPELINE_INFO_PUSH(std::format("{}", observer->getClassName()));

		if (mRenderingPassArray.empty())
		{
			//如果这个空queue连续运行了255帧都没有东西需要渲染
			//就自动移除他
			if (mCountDown == (uint8_t)255)
			{
				mRemoveFlag = true;
			}
			else
			{
				mCountDown++;
			}
			TINY_PIPELINE_INFO_POP();
			return;
		}

		mCountDown = 0;
		if (mDirty)
		{
			mDirty = false;
			std::ranges::sort(mRenderingPassArray.begin(), mRenderingPassArray.end(), [](PipelinePass* a, PipelinePass* b)
			{
				return a->getPipelineSortingID() < b->getPipelineSortingID();
			});
		}


		auto frame_buffer = mRenderObserver->getFrameBuffer();
		if (frame_buffer)
		{
			if (Pipeline::getFrameCount() != frame_buffer->currentFrame())
			{
				frame_buffer->updateCurrentFrame(Pipeline::getFrameCount());
				Graphics::getInstance()->bind(frame_buffer);

				Graphics::getInstance()->setClearColor(mRenderObserver->getClearColor());
				Graphics::getInstance()->clear(mRenderObserver->getClearOption());
				Graphics::getInstance()->setViewport(mRenderObserver->getViewRect());
			}
		}


		auto it = mRenderingPassArray.begin();
		auto end = mRenderingPassArray.end();
		while (it != end)
		{
			auto pass = *it;
			if (pass->isNeedRemoved())
			{
				it = mRenderingPassArray.erase(it);
				end = mRenderingPassArray.end();

				pass->onExitPipeline();
				pass->deleteObject();
			}
			else
			{
				pass->render(observer);
				it++;
			}
		}

		if (frame_buffer)
		{
			Graphics::getInstance()->unbind(frame_buffer);
		}
		TINY_PIPELINE_INFO_POP();
	}

	ObserverPipelinePass* PipelineQueue::createOrGetObserverPass(Shader* shader)
	{
		auto uid = shader->getUID();
		while (mPassTable.size() <= uid)
		{
			mPassTable.push_back(nullptr);
		}

		ObserverPipelinePass* pass = mPassTable[uid];
		if (pass == nullptr)
		{
			pass = ObserverPipelinePass::create(mRenderObserver.get(), shader);
			mPassTable[uid] = pass;
			pass->saveObject();
		}

		pass->addToPipeline();
		return pass;
	}

	void PipelineQueue::addToPipeline()
	{
		if (!mIsInPipeline)
		{
			mIsInPipeline = true;
			PipelineManager::addToPipeline(this);
		}
	}

	void PipelineQueue::setObserver(BaseRenderObserver* observer)
	{
		Base::setObserver(observer);
		mSortingID = observer->getSortingID();
		observer->evtSortingIDChanged = [this](int32_t sortingID)
			{
				mSortingID = sortingID;
				PipelineManager::sortQueue();
			};
		mRenderObserver = observer;
	}

	void PipelineQueue::onExitPipeline()
	{
		Base::onExitPipeline();
		for (auto pass : mRenderingPassArray)
		{
			pass->onExitPipeline();
			pass->deleteObject();
		}
		mRenderingPassArray.clear();
	}

	void PipelineQueue::onEnterPipeline()
	{
		Base::onEnterPipeline();
		mCountDown = 0;
	}

	void PipelineQueue::onClose()
	{
		Base::onClose();
		this->onExitPipeline();

		for (auto pass : mPassTable)
		{
			if (pass)
			{
				TINY_ASSERT(pass->getRefCount() == 1);
				pass->deleteObject();
			}
		}
		mPassTable.clear();
	}
}
