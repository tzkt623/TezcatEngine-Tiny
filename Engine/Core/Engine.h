#pragma once
#include <string>
#include "GLHead.h"

struct GLFWwindow;

namespace tezcat::Tiny::Module
{
	class SceneManager;
	class CameraManager;
	class InputSystem;
	class ResourceLoader;
}

namespace tezcat::Tiny::Core
{
	class Renderer;
	class ShaderManager;
	class Engine
	{
	public:
		static Engine* getInstance()
		{
			static Engine instance;
			return &instance;
		}

	private:
		Engine();
		~Engine();

	public:
		int init(Module::ResourceLoader* loader);
		void run();
		void close();

	private:
		void initInputSystem();

	public:
		Renderer* getRenderer() const { return m_Renderer; }
		ShaderManager* getShaderManager() const { return m_ShaderManager; }
		Module::CameraManager* getCameraManager() const { return m_CameraManager; }
		Module::SceneManager* getSceneManager() const { return m_SceneManager; }
		Module::InputSystem* getInputSystem() const { return m_InputSystem; }

	private:
		static void mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods);
		static void mouseCursorPosCallBack(GLFWwindow* window, double xpos, double ypos);
		static void mouseScrollPosCallBack(GLFWwindow* window, double xoffset, double yoffset);
		
	public:
		GLFWwindow* getGLFWWindow() const { return m_Window; }

	private:
		GLFWwindow* m_Window;
		Renderer* m_Renderer;

	private:
		ShaderManager* m_ShaderManager;
		Module::SceneManager* m_SceneManager;
		Module::CameraManager* m_CameraManager;
		Module::InputSystem* m_InputSystem;
		Module::ResourceLoader* m_ResourceLoader;

	public:
		inline static int getScreenWidth() { return ScreenWidth; }
		inline static int getScreenHeight() { return ScreenHeight; }
		inline static float getDeltaTime() { return DeltaTime; }

	private:
		static int ScreenWidth;
		static int ScreenHeight;
		static float DeltaTime;
	};
}