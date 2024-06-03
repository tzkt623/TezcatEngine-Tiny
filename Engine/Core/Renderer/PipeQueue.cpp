#include "PipeQueue.h"
#include "BaseGraphics.h"

#include "Pipeline.h"
#include "FrameBuffer.h"
#include "RenderObserver.h"

#include "../Manager/FrameBufferManager.h"

namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(PipelineQueue, TinyObject)

	PipelineQueue::PipelineQueue()
		: mRenderObserver(nullptr)
		, mFrameBuffer(nullptr)
		, mOrderID(0)
	{

	}

	PipelineQueue::~PipelineQueue()
	{
		if (mRenderObserver)
		{
			mRenderObserver->deleteObject();
		}

		if (mFrameBuffer)
		{
			mFrameBuffer->deleteObject();
		}

		for (auto pass : mArray)
		{
			pass->deleteObject();
		}
	}

	void PipelineQueue::beginQueue(BaseGraphics* graphics)
	{
		if (mDirty)
		{
			mDirty = false;
			std::sort(mArray.begin(), mArray.end(), [](PipelinePass* a, PipelinePass* b)
			{
				return a->getOrderID() < b->getOrderID();
			});
		}

		graphics->buildCMD();
	}

	void PipelineQueue::preRender(BaseGraphics* graphics)
	{
		if (mFrameBuffer != nullptr)
		{
			FrameBufferManager::bind(graphics, mFrameBuffer);
		}
		else
		{
			FrameBufferManager::bind(graphics, mRenderObserver->getFrameBuffer());
		}
	}

	void PipelineQueue::render(BaseGraphics* graphics)
	{
		mRenderObserver->updateObserverMatrix();

		graphics->setViewport(mRenderObserver->getViewportInfo());
		graphics->clear(mRenderObserver->getClearOption());

		for (auto pass : mArray)
		{
			pass->render(graphics);
		}
	}

	void PipelineQueue::postRender(BaseGraphics* graphics)
	{
		if (mFrameBuffer != nullptr)
		{
			FrameBufferManager::unbind(graphics, mFrameBuffer);
		}
		else
		{
			FrameBufferManager::unbind(graphics, mRenderObserver->getFrameBuffer());
		}
	}

	void PipelineQueue::endQueue(BaseGraphics* graphics)
	{
		graphics->buildCMD();
	}

	void PipelineQueue::submitObserverData(BaseGraphics* graphics, Shader* shader)
	{
		mRenderObserver->submitViewMatrix(graphics, shader);
	}

	void PipelineQueue::addPass(PipelinePass* pass)
	{
		mArray.push_back(pass);
		pass->saveObject();
		mDirty = true;
	}

	void PipelineQueue::setRenderObserver(BaseRenderObserver* renderObserver)
	{
		mRenderObserver = renderObserver;
		mRenderObserver->saveObject();
	}

	void PipelineQueue::setFrameBuffer(FrameBuffer* frameBuffer)
	{
		mFrameBuffer = frameBuffer;
		mFrameBuffer->saveObject();
	}

	int PipelineQueue::getOrderID() const
	{
		return mRenderObserver == nullptr ? mOrderID : mRenderObserver->getOrderID();
	}

}
