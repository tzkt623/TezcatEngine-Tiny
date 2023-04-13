#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "Utility/Singleton.h"

using namespace tezcat::Tiny::Utility;
namespace tezcat::Tiny::Core
{
	class BaseGraphics;
	class Shader;
	class Pipeline;
	class RenderLayer;
	class LightLayer;
	class IRenderObject;
	class IRenderMesh;
	class PipelineQueue;
	class ILight;

	enum class PipelinePath : uint32_t
	{
		Forward = 0,
		ForwardPlus,
	};


	class TINY_API PipelineManager
	{
	public:
		PipelineManager();
		~PipelineManager();

		void render(BaseGraphics* graphics);
		void postRender(BaseGraphics* graphics);
		void preRender(BaseGraphics* graphics);

	public:
		LightLayer* getLightLayer(uint32_t index) { return mLightLayerAry[index]; }

	public:
		void addLight(uint32_t layerIndex, ILight* light);

		uint32_t add(const std::string& name, Pipeline* pl);
		Pipeline* get(const std::string& name);



	private:
		std::vector<LightLayer*> mLightLayerAry;
		std::vector<Pipeline*> mPipelineAry;
		std::unordered_map<std::string, uint32_t> mPipelineUMap;
	};

	using PipelineMgr = SG<PipelineManager>;
}
