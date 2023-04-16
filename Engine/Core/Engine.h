#pragma once

#include "Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class SceneManager;
	class CameraManager;
	class InputSystem;
	class ResourceLoader;
	class ShaderManager;
	class LightManager;
	class BaseGraphics;
	class FrameBufferManager;
	class PipelineManager;
	class VertexManager;
	class TextureManager;
	class TINY_API Engine
	{
	public:
		Engine();
		virtual ~Engine();

	public:
		bool init(ResourceLoader* loader);
		void run();
		BaseGraphics* getGraphics() { return mGraphics; }

	protected:
		virtual bool preInit(ResourceLoader* loader);
		virtual bool onInit(ResourceLoader* loader);
		virtual bool postInit(ResourceLoader* loader);

		virtual void stop();
		virtual void beforeLoop();
		virtual void endLoop();

		virtual void preUpdate();
		virtual void postUpdate();
		virtual void onUpdate();

		virtual BaseGraphics* createGraphics() = 0;

	protected:
		ShaderManager* mShaderManager;
		LightManager* mLightManager;
		SceneManager* mSceneManager;
		CameraManager* mCameraManager;
		FrameBufferManager* mFrameBufferManager;
		TextureManager* mTextureManager;
		PipelineManager* mPipelineManager;
		VertexManager* mVertexManager;


		InputSystem* mInputSystem;
		ResourceLoader* mResourceLoader;

	private:
		BaseGraphics* mGraphics;


	protected:
		bool mIsRunning;

	public:
		inline static int getScreenWidth() { return ScreenWidth; }
		inline static int getScreenHeight() { return ScreenHeight; }
		inline static float getDeltaTime() { return sDeltaTime; }
	protected:
		static int ScreenWidth;
		static int ScreenHeight;
		static float sDeltaTime;
	};
}
