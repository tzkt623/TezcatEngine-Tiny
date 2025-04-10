#pragma once
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
#include "../Base/TinyObject.h"
#include "../Head/TinyCpp.h"
#include "../Tool/Tool.h"
#include "PipelineWorker.h"
#include "RenderObserver.h"


namespace tezcat::Tiny
{
	/*
	* Pipeline整体设计
	* 
	* Pipeline:
	*   PipelineQueue: 每一个观察者对应一个Queue
	*     PipelinePass: 每一个Shader对应一个Pass
	*       RenderCommand
	*
	* 每一帧遍历所有queue执行里面的cmd进行渲染
	* Queue的排序
	*	按照相机的OrderID进行排序
	*	OrderID越小越先进行渲染
	*
	* 例如.
	*	前处理ObseverQueue1
	*	......
	*	前处理ObseverQueueN
	* 
	*	玩家相机Queue1
	*	......
	*	玩家相机QueueN
	* 
	*	后处理ObseverQueue1
	*	......
	*	后处理ObseverQueueN
	*/

	class TINY_API Pipeline
	{
	public:
		Pipeline();
		virtual ~Pipeline() = default;

		virtual void init();
		virtual void render();
		virtual void addPipelineQueue(PipelineQueue* queue) = 0;

		static uint32_t getFrameCount() { return sFrameCount; }

		void setQueueDirty() { mDirty = true; }

	protected:
		void sortQueue();

		virtual void preRender() = 0;
		virtual void onRender() = 0;
		virtual void postRender() = 0;

	protected:
		bool mDirty;
		std::vector<PipelineQueue*> mQueueArray;

	protected:
		static uint32_t sFrameCount;
	};


	/*
	* 当管线开始渲染时,他需要确定一些东西
	* 1.环境光照,他需要一个6面正方形相机
	* 2.阴影效果,不同的灯光需要不同的相机
	* 3.游戏画面,玩家视口相机
	*	a.正向渲染
	*	b.延迟渲染
	*	c.后处理效果
	* 4.屏幕空间效果
	*
	* 所以管线的顺序应该可以让用户自己安排
	*/
	class TINY_API PipelineBuildin : public Pipeline
	{
	public:
		PipelineBuildin();
		virtual ~PipelineBuildin() noexcept;
		virtual void addPipelineQueue(PipelineQueue* queue) override;

	protected:
		virtual void preRender() override;
		virtual void onRender() override;
		virtual void postRender() override;

	protected:
	};
}
