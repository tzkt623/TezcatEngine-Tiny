#pragma once

#include "../Renderer/RenderObject.h"
#include "../Tool/Tool.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	class BaseGraphics;
	class Skybox;
	class TextureCube;
	class TextureRender2D;
	class Texture2D;
	class Image;
	class FrameBuffer;
	class RenderCommand;
	class TINY_API EnvObserver : public TinyObject, public IRenderObserver
	{
		EnvObserver();
		TINY_Factory(EnvObserver);
		TINY_RTTI_H(EnvObserver);
	public:
		virtual ~EnvObserver();

		void submit(BaseGraphics* graphics, Shader* shader) override;
		void submitViewMatrix(BaseGraphics* graphics, Shader* shader) override;
	};

	/*
	* EnvironmentLightManager
	* 
	* @brief 环境光照管理器
	*/
	class TINY_API EnvironmentLightManager
	{
	public:
		EnvironmentLightManager();
		~EnvironmentLightManager();

		void init();

		void calculate(BaseGraphics* graphics);
		void submit(BaseGraphics* graphics, Shader* shader);
		void setHDRImage(Image* image);

		Texture2D* getHDRTexture() { return mTexHDR; }

		RenderCommand* createSkyboxCMD(BaseGraphics* graphics);

	public:
		void showIrradianceMap();
		void showPrefilterMap();
		void showSkybox();
		void setSkyboxLod(float skyboxLod);

	private:
		void createCube();
		void createIrradiance();
		void createPrefilter();
		void createBRDF_LUT();

	private:
		bool mDirty;

		uint32_t mCubeSize;
		uint32_t mIrrSize;
		uint32_t mPrefilterSize;

		TextureCube* mCubeMap;
		TextureCube* mIrradianceMap;
		TextureCube* mPrefilterMap;
		Texture2D* mBRDFLUTMap;

		FrameBuffer* mCubeFB;
		FrameBuffer* mIrradianceFB;
		FrameBuffer* mPrefilterFB;

		EnvObserver* mCubeObserver;
		EnvObserver* mIrradianceObserver;
		EnvObserver* mPrefilterObserver;
		EnvObserver* mBRDFLUTObserver;

		Texture2D* mTexHDR;
		Texture2D* mCubeTextures[6];

		Shader* mSkyboxShader;
		Vertex* mSkyboxVertex;
		float mSkyboxLod;

		TextureCube* mCurrentCubeMap;
	};

	using EnvLitMgr = SG<EnvironmentLightManager>;
}
