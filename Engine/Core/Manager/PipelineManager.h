#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "Utility/Singleton.h"

using namespace tezcat::Tiny::Utility;
namespace tezcat::Tiny::Core
{
	class BaseGraphics;
	class Shader;
	class RenderPass;
	class Pipeline;
	class Camera;
	class RenderLayer;
	class LightLayer;
	class IRenderObject;
	class ILight;
	class TINY_API PipelineManager
	{
	public:
		PipelineManager();
		~PipelineManager();

	public:
		void createPass(Shader* shader);

		RenderPass* getPass(int uid) { return mPassList[uid]; }
		void loadAllShaderToRenderer(BaseGraphics* graphics);

		RenderLayer* getRenderLayer(uint32_t index) { return mRenderLayerList[index]; }
		LightLayer* getLightLayer(uint32_t index) { return mLightLayerList[index]; }
		void addRenderObject(uint32_t layerIndex, IRenderObject* renderObject);
		void addLight(uint32_t layerIndex, ILight* light);

		void add(const std::string& name, Pipeline* pl);
		Pipeline* get(const std::string& name);

	private:
		std::vector<RenderPass*> mPassList;
		std::vector<std::tuple<Camera*, Pipeline*>> s_PipelineArray;


		std::vector<RenderLayer*> mRenderLayerList;
		std::vector<LightLayer*> mLightLayerList;

		std::unordered_map<std::string, Pipeline*> sPipelineMap;
	};

	using PipelineMgr = SG<PipelineManager>;
}
