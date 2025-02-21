#pragma once

#include "../Tool/Tool.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	class Skybox;
	class Texture;
	class TextureCube;
	class TextureRender2D;
	class Texture2D;
	class Image;
	class FrameBuffer;
	class RenderCommand;
	class Pipeline;
	class Vertex;
	class Shader;
	class ReplacedPipelinePass;
	class RenderObserverMultView;

	class DirectionalLight;
	class PointLight;
	class SpotLight;
	class ILight;

	class TINY_API LightData : public TinyObject
	{
		LightData();
		TINY_OBJECT_H(LightData, TinyObject)
	public:
		virtual ~LightData();

	public:
		DirectionalLight* directionalLight;
		std::vector<PointLight*> pointLights;
		std::vector<SpotLight*> spotLights;
	};

	/*
	* EnvironmentLightManager
	* 
	* @brief 环境光照管理器
	*/
	class TINY_API LightingManager
	{
		LightingManager() = delete;
		~LightingManager() = delete;
	public:
		static void init();
		static void close();


	public:
		//----------------------------------------
		//
		//	Normal Lighting
		//
		static void setLightData(LightData* data);
		static void submitLighting(Shader* shader);
		static void setDirectionalLight(DirectionalLight* agent);

	public:
		//-----------------------------------------
		//
		// Environment Lighting
		//
		static void calculate();
		static void submitEnvLighting(Shader* shader);
		static void setSkyBoxHDRTexture(Image* image);
		static Texture2D* getSkyBoxHDRTexture() { return sTexHDR; }
		static auto getCubeMapTextureArray() { return sCubeTextures; }

	public:
		static void enableSkyBox() { sEnableSkyBox = true; }
		static void disableSkyBox() { sEnableSkyBox = false; }
		static void showSkybox();
		static void setSkyboxLod(float skyboxLod);
		static void setSkyBox(const std::array<Image*, 6>& images);
		static float getExposure() { return sExposure; }
		static void setExposure(float val) { sExposure = val; }

	public:
		static void showIrradianceMap();
		static void showPrefilterMap();

		static void enableEnvLighting()
		{
			sCalculateEnvLighting = true;
		}

		static void disableEnvLighting()
		{
			sCalculateEnvLighting = false;
		}

	private:
		static RenderObserverMultView* createObserver();
		static void createHDR2Cube();
		static void createIrradiance();
		static void createPrefilter();
		static void createBRDF_LUT();

	private:
		static uint32 sCubeSize;
		static uint32 sIrrSize;
		static uint32 sPrefilterSize;

		static TextureCube* sCubeTextureMap;
		static TextureCube* sIrradianceMap;
		static TextureCube* sPrefilterMap;
		static Texture2D* sBRDFLUTMap;

		static Texture2D* sTexHDR;
		static Texture2D* sCubeTextures[6];

		Shader* mSkyboxShader;
		static Vertex* sSkyboxVertex;
		static Vertex* sHDRVertex;
		static float sSkyboxLod;
		static float sExposure;

		static TextureCube* sCurrentCubeMap;

	private:
		static bool sCalculateEnvLighting;
		static bool sCloseEnvLighting;
		static bool sEnableSkyBox;
		static bool sIsSceneExited;

	private:
		static ReplacedPipelinePass* sMakeCubeTexPass;
		static ReplacedPipelinePass* sIrradiancePass;
		static ReplacedPipelinePass* sPrefilterPass;
		static ReplacedPipelinePass* sBRDFLUTPass;
		static ReplacedPipelinePass* sSkyBoxPass;
		static std::vector<std::function<void()>> mEnvPassArray;

	private:
		static LightData* sLightData;
		static std::function<void()> sRemovePassFromPipeline;

	};
}
