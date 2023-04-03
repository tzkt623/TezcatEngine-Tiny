#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class BaseGraphics;
	class Shader;
	class RenderPass;
	class Pipeline;
	class Camera;
	class RenderLayer;
	class LightLayer;
	class IRenderObejct;
	class ILight;
	class TINY_API PipelineManager
	{
	public:
		static void createPass(Shader* shader);

		static RenderPass* getPass(int uid) { return mPassList[uid]; }
		static void loadAllShaderToRenderer(BaseGraphics* graphics);

		static RenderLayer* getRenderLayer(uint32_t index) { return mRenderLayerList[index]; }
		static LightLayer* getLightLayer(uint32_t index) { return mLightLayerList[index]; }
		static void addRenderObject(uint32_t layerIndex, IRenderObejct* renderObject);
		static void addLight(uint32_t layerIndex, ILight* light);

		static void add(const std::string& name, Pipeline* pl);
		static Pipeline* get(const std::string& name);

	private:
		static std::vector<RenderPass*> mPassList;
		static std::vector<std::tuple<Camera*, Pipeline*>> s_PipelineArray;


		static std::vector<RenderLayer*> mRenderLayerList;
		static std::vector<LightLayer*> mLightLayerList;

		static std::unordered_map<std::string, Pipeline*> sPipelineMap;
	};
}
