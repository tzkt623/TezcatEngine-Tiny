#pragma once

#include "RenderObject.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class TINY_API ShadowCast : public RenderPass
	{
	public:
		ShadowCast();
		virtual ~ShadowCast();

		
		void init() override;

		void render(BaseGraphics* graphics, Camera* camera) override;
		void render(BaseGraphics* graphics, IRenderObserver* renderObject) override;

		void addPass(RenderPass* pass) override;

	private:
		bool mDirty;
		std::vector<RenderPass*> mRenderPassAry;
		std::unordered_map<uint32_t, RenderPass*> mRenderPassUMap;
	};
}
