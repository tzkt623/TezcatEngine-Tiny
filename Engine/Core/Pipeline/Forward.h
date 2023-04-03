#pragma once

#include "Pipeline.h"
#include "..\Head\ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class PipelineQueue;
	class TINY_API Forward : public Pipeline
	{
	public:
		Forward();
		~Forward();

		PipelineType getType() final { return PipelineType::Forward; }

		void init() override;

		void render(Camera* camera) override;
		void addPass(RenderPass* pass) override;

	private:
		PipelineQueue* mBackground;
		PipelineQueue* mGeometry;
		PipelineQueue* mAlphaTest;
		PipelineQueue* mTransparent;
		PipelineQueue* mOverlay;
	};
}
