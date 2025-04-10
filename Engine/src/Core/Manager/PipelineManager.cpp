#include "Core/Manager/PipelineManager.h"
#include "Core/Renderer/CameraObserver.h"
#include "Core/Renderer/Pipeline.h"
#include "Core/Renderer/RenderObserver.h"
#include "Core/Shader/Shader.h"
#include "Core/Event/EngineEvent.h"


namespace tezcat::Tiny
{
	Pipeline* PipelineManager::sCurrentPipeline = nullptr;
	std::vector<PipelineQueue*> PipelineManager::sObserverQueueTable;

	void PipelineManager::addToPipeline(PipelineQueue* queue)
	{
		sCurrentPipeline->addPipelineQueue(queue);
	}

	void PipelineManager::initPipeline(Pipeline* pipeline)
	{
		sCurrentPipeline = pipeline;
	}

	void PipelineManager::init()
	{
		EngineEvent::getInstance()->addListener(EngineEventID::EE_BeforeSceneExit, sCurrentPipeline
			, [](const EventData& data)
			{
				//for (auto queue : sObserverQueueTable)
				//{
				//
				//}
			});
	}

	void PipelineManager::sortQueue()
	{
		sCurrentPipeline->setQueueDirty();
	}
}
