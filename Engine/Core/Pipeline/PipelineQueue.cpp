#include "PipelineQueue.h"
#include "../Renderer/RenderPass.h"
#include "../Component/Camera.h"
#include "../Component/Light.h"
#include "../Manager/TextureManager.h"

namespace tezcat::Tiny::Core
{
	std::unordered_map<std::string, PipelineQueue::Queue> PipelineQueue::sQueueMap =
	{
		{"None",		Queue::None},
		{"Background",	Queue::Background},
		{"Opaque",		Queue::Geometry},
		{"AlphaTest",	Queue::AlphaTest},
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
		this->sort();

		for (auto pass : mRenderPassAry)
		{
			if (pass->checkState())
			{
				camera->submitViewMatrix(pass->getShader());
				pass->render(graphics);
			}
		}
	}

	void PipelineQueue::render(BaseGraphics* graphics, IRenderObserver* observer, ILight* light)
	{
		this->sort();

		for (auto pass : mRenderPassAry)
		{
			if (pass->checkState())
			{
				observer->submitViewMatrix(pass->getShader());
				if (light)
				{
					light->submitViewMatrix(pass->getShader());
					light->submit(pass->getShader());

					auto tex = TextureMgr::getInstance()->findTexture("Shadow");
					if (tex != nullptr)
					{
						pass->getShader()->setTexture2D(ShaderParam::TexDepth, (Texture2D*)tex);
					}
				}
				pass->render(graphics);
			}
		}
	}

	void PipelineQueue::addPass(RenderPass* pass)
	{
		mDirty = true;
		mRenderPassAry.push_back(pass);
	}

	void PipelineQueue::sort()
	{
		if (mDirty)
		{
			mDirty = false;
			std::sort(mRenderPassAry.begin(), mRenderPassAry.end()
				, [](RenderPass* a, RenderPass* b)
				{
					return a->getOrderID() < b->getOrderID();
				});
		}
	}
}
