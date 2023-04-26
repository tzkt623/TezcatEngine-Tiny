#pragma once

#include "Pipeline.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny
{
	class PipelineQueue;
	class RenderPass;

	/*
	* @author: HCL
	* @time: 4/9/2023
	* 前向渲染
	* 基本的前向渲染流程应该是先渲染阴影
	* 1.预渲染部分
	*	1.对于每一个灯,渲染一遍阴影数据
	* 2.渲染
	*	1.Background
	*	2.Geometry
	*	3.AlphaTest
	*	4.OpaqueLast
	*	4.Transparent
	*	5.Overlay
	* 3.后渲染
	*/
	class TINY_API Forward : public Pipeline
	{
	public:
		Forward();
		virtual ~Forward();

		PipelineType getType() final { return PipelineType::Forward; }

		void render(BaseGraphics* graphics, Camera* camera) override;
		void render(BaseGraphics* graphics, IRenderObserver* renderObject) override;
	};
}
