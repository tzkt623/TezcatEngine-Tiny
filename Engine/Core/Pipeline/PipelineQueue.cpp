#include "PipelineQueue.h"
#include "../Renderer/RenderPass.h"
#include "../Component/Camera.h"

namespace tezcat::Tiny::Core
{
	std::unordered_map<std::string, PipelineQueue::Queue> PipelineQueue::sQueueMap =
	{
		{"Background",	Queue::Background},
		{"Opaque",		Queue::Geometry},
		{"Alpha",		Queue::AlphaTest},
		{"OpaqueLast",	Queue::OpaqueLast},
		{"Transparent", Queue::Transparent},
		{"Overlay",		Queue::Overlay}
	};

	PipelineQueue::PipelineQueue(const Queue& queue, const uint32_t& baseOrderID)
		: mQueueID(queue)
		, mBaseOrderID(baseOrderID)
		, mDirty(true)
	{

	}

	void PipelineQueue::render(BaseGraphics* graphics, Camera* camera)
	{
		if (mDirty)
		{
			mDirty = false;
			std::sort(mShaderList.begin(), mShaderList.end()
				, [](RenderPass* a, RenderPass* b)
				{
					return a->getOrderID() < b->getOrderID();
				});
		}

		for (auto pass : mShaderList)
		{
			pass->render(graphics, camera);
		}
	}

	void PipelineQueue::addPass(RenderPass* pass)
	{
		mShaderList.push_back(pass);
		mDirty = true;
	}
}
