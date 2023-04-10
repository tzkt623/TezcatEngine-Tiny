#pragma once
#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"


namespace tezcat::Tiny::Core
{
	enum class TINY_API PipelineType
	{
		Error,
		Forward,
		Deferred,
		ForwardPlus
	};

	class Camera;
	class RenderPass;
	class BaseGraphics;
	class IRenderObserver;

	class TINY_API Pipeline
	{
	public:
		Pipeline();
		~Pipeline();

		virtual PipelineType getType() { return PipelineType::Error; }

		virtual void init() = 0;
		virtual void render(BaseGraphics* graphics, Camera* camera) = 0;
		virtual void render(BaseGraphics* graphics, IRenderObserver* renderObject) {}
		virtual void addPass(RenderPass* pass) = 0;

		virtual void setShadowCast() = 0;
	};
}
