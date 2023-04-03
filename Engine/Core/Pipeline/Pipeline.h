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


	class TINY_API Pipeline
	{
	public:
		Pipeline();
		~Pipeline();

		virtual PipelineType getType() { return PipelineType::Error; }

		virtual void init() = 0;
		virtual void render(Camera* camera) = 0;
		virtual void addPass(RenderPass* pass) = 0;

		BaseGraphics* getGraphics() const { return mGraphics; }
		void setGraphics(BaseGraphics* val) { mGraphics = val; }

	protected:
		BaseGraphics* mGraphics;
	};
}
