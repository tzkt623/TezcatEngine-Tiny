#pragma once
/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
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
	class RenderObserver;
	class RenderObserverMultView;

	class DirectionalLight;
	class PointLight;
	class SpotLight;
	class ILight;

	/*
	* LightData
	* 此类并不负责其中对象的生命周期管理
	*/
	class TINY_API LightData : public TinyObject
	{
		LightData();
		TINY_OBJECT_H(LightData, TinyObject)
	public:
		virtual ~LightData();

	protected:
		void onClose() override;

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

		static RenderObserverMultView* sObserverHDR;
		static RenderObserverMultView* sObserverIrradiance;
		static RenderObserverMultView* sObserverPrefilter;
		static RenderObserver* sObserverBRDF_LUT;

	private:
		static LightData* sLightData;
		static std::function<void()> sRemovePassFromPipeline;

	};
}
