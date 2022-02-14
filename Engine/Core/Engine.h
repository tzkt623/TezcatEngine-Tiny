#pragma once

#include "Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class SceneManager;
	class CameraManager;
	class InputSystem;
	class ResourceLoader;
	class Renderer;
	class ShaderManager;
	class LightManager;
	class BaseGraphics;
	class TINY_API Engine
	{
	public:
		Engine();
		virtual ~Engine();

	public:
		bool init(ResourceLoader* loader);
		void run();

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
		Renderer* m_Renderer;
		ShaderManager* m_ShaderManager;
		LightManager* m_LightManager;
		SceneManager* m_SceneManager;
		CameraManager* m_CameraManager;
		InputSystem* m_InputSystem;
		ResourceLoader* m_ResourceLoader;

	private:
		BaseGraphics* m_Graphics;

	protected:
		bool m_IsRunning;

	public:
		inline static int getScreenWidth() { return ScreenWidth; }
		inline static int getScreenHeight() { return ScreenHeight; }
		inline static float getDeltaTime() { return DeltaTime; }
	protected:
		static int ScreenWidth;
		static int ScreenHeight;
		static float DeltaTime;
	};
}