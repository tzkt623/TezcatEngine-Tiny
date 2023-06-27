#pragma once

#include "Head/ConfigHead.h"
#include "Head/TinyCpp.h"

namespace tezcat::Tiny
{
	class SceneManager;
	class CameraManager;
	class InputSystem;
	class EngineIniter;
	class ShaderManager;
	class LightManager;
	class BaseGraphics;
	class FrameBufferManager;
	class VertexBufferManager;
	class ModelManager;
	class TextureManager;

	class TINY_API Engine
	{
	public:
		Engine();
		virtual ~Engine();

	public:
		bool init(EngineIniter* loader);
		void run();
		BaseGraphics* getGraphics() { return mGraphics; }
		EngineIniter* getResourceLoader() { return mResourceLoader; }
		void setEnableMultiThread() {
			sMultiThread = true;
		}

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
		virtual void onRender();
		virtual void notifyRender();
		virtual void notifyRenderThread();

		virtual BaseGraphics* createGraphics() = 0;

	protected:
		ShaderManager* mShaderManager;
		SceneManager* mSceneManager;
		FrameBufferManager* mFrameBufferManager;
		TextureManager* mTextureManager;
		VertexBufferManager* mVertexManager;
		ModelManager* mModelManager;
		InputSystem* mInputSystem;
		EngineIniter* mResourceLoader;
		BaseGraphics* mGraphics;

	protected:
		bool mIsRunning;
		std::thread mRenderThread;
		std::condition_variable mThreadCondition;
		std::atomic<bool> mRenderThreadInited;

	public:
		inline static const int& getScreenWidth() { return ScreenWidth; }
		inline static const int& getScreenHeight() { return ScreenHeight; }
		inline static const float& getDeltaTime() { return sDeltaTime; }
		inline static const std::string& getName() { return sName; }

	protected:
		static std::string sName;
		static int ScreenWidth;
		static int ScreenHeight;
		static float sDeltaTime;

	public:
		static bool sMultiThread;

	private:
		static std::atomic<bool> allowRender;
		static std::atomic<bool> allowLogic;
		static std::mutex sMainMutex;
		static std::condition_variable sCVRenderThread;
		static std::condition_variable sCVLogicThread;
		static std::binary_semaphore sSemRenderThread;
		static std::binary_semaphore sSemLogicThread;

	};
}
