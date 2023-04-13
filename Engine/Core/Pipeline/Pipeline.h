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
	class PipelineQueue;

	class TINY_API Pipeline
	{
	public:
		Pipeline();
		virtual ~Pipeline();

		virtual PipelineType getType() { return PipelineType::Error; }

		virtual void render(BaseGraphics* graphics, Camera* camera) = 0;
		virtual void render(BaseGraphics* graphics, IRenderObserver* renderObject) = 0;
		void addPass(RenderPass* pass);

	public:
		static void addPassStatic(RenderPass* pass);

	protected:
		static PipelineQueue* sBackground;
		static PipelineQueue* sGeometry;
		static PipelineQueue* sAlphaTest;
		static PipelineQueue* sOpaqueLast;
		static PipelineQueue* sTransparent;
		static PipelineQueue* sOverlay;
	};
}
