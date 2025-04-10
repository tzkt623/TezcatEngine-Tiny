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
#include "Core/Manager/SceneManager.h"
#include "Core/Manager/PipelineManager.h"

#include "Core/Debug/Debug.h"

#include "Core/Event/EngineEvent.h"


namespace tezcat::Tiny
{
	//-------------------------------------------
	//
	//	Pipeline
	//
	uint32_t Pipeline::sFrameCount = 0;

	Pipeline::Pipeline()
		: mDirty(true)
	{
		PipelineManager::initPipeline(this);
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
		//if (SceneManager::empty())
		//{
		//	return;
		//}

		TINY_PROFILER_TIMER_OUT(Profiler::RenderTime);
		TINY_PIPELINE_INFO_PUSH("Pipeline");

		this->preRender();
		this->onRender();
		this->postRender();

		TINY_PIPELINE_INFO_POP();
	}

	void Pipeline::sortQueue()
	{
		//对queue进行排序
		if (mDirty)
		{
			mDirty = false;
			std::ranges::sort(mQueueArray.begin(), mQueueArray.end(), [](PipelineQueue* a, PipelineQueue* b)
			{
				return a->getSortingID() > b->getSortingID();
			});
		}
	}

	//-------------------------------------------
	//
	//	PipelineBuildin
	//
	PipelineBuildin::PipelineBuildin()
	{
		EngineEvent::getInstance()->addListener(EngineEventID::EE_BeforeSceneExit, this
			, [this](const EventData& data)
			{
				ReplacedPipelinePass::clearPassArray();

				for (auto queue : mQueueArray)
				{
					queue->onExitPipeline();
					queue->deleteObject();
				}

				mQueueArray.clear();
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

		CameraManager::preRender();
		LightingManager::preRender();
		GameObjectManager::preRender();

		//ReplacedPipelinePass必须在最后
		//因为前面的流程可能生成此Pass
		ReplacedPipelinePass::preRender();

		Graphics::getInstance()->buildCommand();

		this->sortQueue();

		Graphics::getInstance()->preRender();
	}

	void PipelineBuildin::onRender()
	{
		auto it = mQueueArray.begin();
		auto end = mQueueArray.end();
		while (it != end)
		{
			auto queue = (*it);
			if (queue->isNeedRemoved())
			{
				queue->onExitPipeline();
				queue->deleteObject();
				it = mQueueArray.erase(it);
				end = mQueueArray.end();
			}
			else
			{
				queue->render();
				it++;
			}
		}
	}

	void PipelineBuildin::postRender()
	{
		sFrameCount++;
		Graphics::getInstance()->postRender();
	}

	void PipelineBuildin::addPipelineQueue(PipelineQueue* queue)
	{
		mDirty = true;
		queue->onEnterPipeline();
		queue->saveObject();
		mQueueArray.push_back(queue);
	}
}
