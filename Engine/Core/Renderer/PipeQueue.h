#pragma once

#include "../Base/TinyObject.h"
#include "../Tool/Tool.h"

namespace tezcat::Tiny
{
	class BaseGraphics;
	class BaseRenderObserver;
	class Shader;
	class FrameBuffer;
	class PipelinePass;

	/*
	* 渲染队列
	* 用于
	* 1.向驱动提交全局数据
	* 2.管理渲染通道并对其排序
	*
	* 用户可以继承此类做出自己的渲染队列
	*/
	class TINY_API PipelineQueue : public TinyObject
	{
	protected:
		PipelineQueue();
		TINY_OBJECT_H(PipelineQueue, TinyObject)

	public:
		virtual ~PipelineQueue();

		void addPass(PipelinePass* pass);

		virtual void beginQueue(BaseGraphics* graphics);
		virtual void preRender(BaseGraphics* graphics);
		virtual void render(BaseGraphics* graphics);
		virtual void postRender(BaseGraphics* graphics);
		virtual void endQueue(BaseGraphics* graphics);

		void setRenderObserver(BaseRenderObserver* renderObserver);

		void setFrameBuffer(FrameBuffer* frameBuffer);

		void setOrderID(int orderID)
		{
			mOrderID = orderID;
		}

		int getOrderID() const;

		int getUID() { return mUID; }

		virtual void submitObserverData(BaseGraphics* graphics, Shader* shader);

		virtual BaseRenderObserver* getRenderObserver() { return mRenderObserver; }

	protected:
		bool mDirty;
		int mUID;
		int mOrderID;

		BaseRenderObserver* mRenderObserver;
		FrameBuffer* mFrameBuffer;
		std::vector<PipelinePass*> mArray;
	};
}
