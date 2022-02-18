#include "WindowsEngine.h"
#include "Core/Manager/LightManager.h"
#include "Core/Manager/ShaderManager.h"
#include "Core/Manager/SceneManager.h"
#include "Core/Manager/CameraManager.h"
#include "Core/Data/ResourceLoader.h"
#include "Core/Input/InputSystem.h"
#include "Core/Scene/LayerMask.h"
#include "Core/Scene/GameObject.h"
#include "../OpenGL/GLGraphics.h"
#include "Core/GUI/GUI.h"




namespace tezcat::Tiny::Core
{
	WindowsEngine::WindowsEngine()
		: m_Window(nullptr)
		, m_NowTime(0)
		, m_OldTime(0)
	{

	}

	WindowsEngine::~WindowsEngine()
	{
		glfwTerminate();
	}

	bool WindowsEngine::preInit(ResourceLoader* loader)
	{
		return Engine::preInit(loader);
	}

	bool WindowsEngine::onInit(ResourceLoader* loader)
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow(ScreenWidth, ScreenHeight, loader->getName().c_str(), nullptr, nullptr);
		if (m_Window == nullptr)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}


		return Engine::onInit(loader);
	}

	bool WindowsEngine::postInit(ResourceLoader* loader)
	{
		m_InputSystem->setWindow(m_Window);
		return Engine::postInit(loader);
	}

	BaseGraphics* WindowsEngine::createGraphics()
	{
		switch (EngineConfig::getRenderAPI())
		{
		case RenderAPI::OpenGL:
		{
			return new GLGraphics();
		}
		default:
			throw std::logic_error("RenderAPI must not null");
			break;
		}
	}
	void WindowsEngine::beforeLoop()
	{
		Engine::beforeLoop();
		m_OldTime = glfwGetTime();
		m_NowTime = 0;
	}

	void WindowsEngine::preUpdate()
	{
		glfwPollEvents();
		m_NowTime = glfwGetTime();
		DeltaTime = static_cast<float>(m_NowTime - m_OldTime);
		m_OldTime = m_NowTime;
	}

	void WindowsEngine::postUpdate()
	{
		GameObject::clearDeletedGameObjects();
		m_IsRunning = !glfwWindowShouldClose(m_Window);
	}

	void WindowsEngine::endLoop()
	{

	}

	void WindowsEngine::onUpdate()
	{
		Engine::onUpdate();
	}

	void WindowsEngine::mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods)
	{
		SG<InputSystem>::getInstance()->mouseButtonCallBack(button, action, mods);
	}

	void WindowsEngine::mouseCursorPosCallBack(GLFWwindow* window, double xpos, double ypos)
	{
		SG<InputSystem>::getInstance()->mouseCursorPosCallBack(xpos, ypos);
	}

	void WindowsEngine::mouseScrollPosCallBack(GLFWwindow* window, double xoffset, double yoffset)
	{
		SG<InputSystem>::getInstance()->mouseScrollPosCallBack(xoffset, yoffset);
	}
}