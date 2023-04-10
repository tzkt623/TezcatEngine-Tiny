#pragma once

#include "Pipeline.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class PipelineQueue;



	/*
	* @author: HCL
	* @time: 4/9/2023
	* 前向渲染
	* 基本的前向渲染流程应该是先渲染阴影
	* 1.预渲染部分
	*	1.渲染阴影数据
	* 2.渲染
	*	1.Background
	*	2.Geometry
	*	3.AlphaTest
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

		void init() override;

		void render(BaseGraphics* graphics, Camera* camera) override;
		void addPass(RenderPass* pass) override;

	private:

		PipelineQueue* mBackground;
		PipelineQueue* mGeometry;
		PipelineQueue* mAlphaTest;
		PipelineQueue* mTransparent;
		PipelineQueue* mOverlay;
	};

	class PipeShadow
	{

	};

}
