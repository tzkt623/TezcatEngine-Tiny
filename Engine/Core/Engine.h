#pragma once

#include "Head/ConfigHead.h"

namespace tezcat::Tiny
{
	class SceneManager;
	class CameraManager;
	class InputSystem;
	class ResourceLoader;
	class ShaderManager;
	class LightManager;
	class BaseGraphics;
	class FrameBufferManager;
	class VertexBufferManager;
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
		virtual bool preInit();
		virtual bool onInit();
		virtual bool postInit();

		virtual void stop();
		virtual void beforeLoop();
		virtual void endLoop();

		virtual void preUpdate();
		virtual void postUpdate();
		virtual void onUpdate();

		virtual BaseGraphics* createGraphics() = 0;

	protected:
		ShaderManager* mShaderManager;
		SceneManager* mSceneManager;
		FrameBufferManager* mFrameBufferManager;
		TextureManager* mTextureManager;
		VertexBufferManager* mVertexManager;
		InputSystem* mInputSystem;
		ResourceLoader* mResourceLoader;
		BaseGraphics* mGraphics;

	protected:
		bool mIsRunning;

	public:
		inline static const int& getScreenWidth() { return ScreenWidth; }
		inline static const int& getScreenHeight() { return ScreenHeight; }
		inline static const float& getDeltaTime() { return sDeltaTime; }

	protected:
		static int ScreenWidth;
		static int ScreenHeight;
		static float sDeltaTime;
	};
}
