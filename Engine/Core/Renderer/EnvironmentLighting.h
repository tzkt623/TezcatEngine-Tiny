#pragma once

#include "RenderObject.h"
#include "../Tool/Tool.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	class BaseGraphics;
	class Skybox;
	class TextureCube;
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
		void calculate(BaseGraphics* graphics);
		void setDirty(const std::string& texName);
		void submit(Shader* shader);

	private:
		bool mDirty;
		std::string mHDRTexName;

		TextureCube* mCubeMap;
		TextureCube* mEnvMap;

		FrameBuffer* mToCubeFB;
		FrameBuffer* mToEnvFB;

		EnvObserver* mToCubeObserver;
		EnvObserver* mToEnvObserver;


		Skybox* mSkybox;
	};

	using EnvLighting = SG<EnvironmentLighting>;
}
