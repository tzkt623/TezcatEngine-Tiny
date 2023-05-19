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
	class FrameBuffer;
	class TINY_API EnvObserver : public TinyObject, public IRenderObserver
	{
		EnvObserver();
		TINY_Factory(EnvObserver);
		TINY_RTTI_H(EnvObserver);
	public:
		virtual ~EnvObserver();

		void submit(Shader* shader) override;
		void submitViewMatrix(Shader* shader) override;
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
		void submit(Shader* shader);

	public:
		void showIrradianceMap();
		void showPrefilterMap();
		void showSkybox();

	private:
		void createCube();
		void createIrradiance();
		void createPrefilter();
		void createBRDF_LUT();

	private:
		bool mDirty;
		std::string mHDRTexName;

		uint32_t mCubeSize;
		uint32_t mIrrSize;

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

		Skybox* mSkybox;
	};

	using EnvLitMgr = SG<EnvironmentLightManager>;
}
