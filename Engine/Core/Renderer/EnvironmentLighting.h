#pragma once

#include "RenderObject.h"
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

	class TINY_API EnvironmentLighting
	{
	public:
		EnvironmentLighting();
		~EnvironmentLighting();

		void init();

		void createBRDF_LUT();

		void calculate(BaseGraphics* graphics);
		void setDirty(const std::string& texName);
		void submit(Shader* shader);
		void showIrradianceMap();
		void showPrefilterMap();
		void showSkybox();

	private:
		void createCube();
		void createIrradiance();
		void createPrefilter();
	private:
		bool mDirty;
		std::string mHDRTexName;

		uint32_t mCubeSize;
		uint32_t mIrrSize;

		TextureCube* mCubeMap;
		TextureCube* mIrradianceMap;
		TextureCube* mPrefilterMap;
		Texture2D* mBRDFLUTMap;
		TextureRender2D* mRender2D;

		FrameBuffer* mCubeFB;
		FrameBuffer* mIrradianceFB;
		FrameBuffer* mPrefilterFB;

		EnvObserver* mCubeObserver;
		EnvObserver* mIrradianceObserver;
		EnvObserver* mPrefilterObserver;
		EnvObserver* mBRDFLUTObserver;


		Skybox* mSkybox;
		
	};

	using EnvLighting = SG<EnvironmentLighting>;
}
