#include "PipelineQueue.h"
#include "../Renderer/RenderPass.h"

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

	void PipelineQueue::render(Camera* camera)
	{
		if (mDirty)
		{
			mDirty = false;
			std::sort(mShaderList.begin()
				, mShaderList.end()
				, [](RenderPass* a, RenderPass* b)
				{
					return a->getOrderID() < b->getOrderID();
				});
		}

		for (auto pass : mShaderList)
		{
			pass->render(camera);
		}
	}

	void PipelineQueue::addPass(RenderPass* pass)
	{
		pass->attach();
		mShaderList.push_back(pass);
		mDirty = true;
	}
}