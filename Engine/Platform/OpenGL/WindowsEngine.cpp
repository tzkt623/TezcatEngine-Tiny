#include "WindowsEngine.h"
#include "Core/Manager/LightManager.h"
#include "Core/Manager/ShaderManager.h"
#include "Core/Manager/SceneManager.h"
#include "Core/Manager/CameraManager.h"
#include "Core/Data/ResourceLoader.h"
#include "Core/Input/InputSystem.h"
#include "Core/Renderer/LayerMask.h"
#include "Core/Component/GameObject.h"
#include "Core/GUI/GUI.h"

#include "../OpenGL/GLGraphics.h"




namespace tezcat::Tiny::GL
{
	WindowsEngine::WindowsEngine()
		: mWindow(nullptr)
		, mTimeNow(0)
		, mTimeOld(0)
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

		mWindow = glfwCreateWindow(ScreenWidth, ScreenHeight, loader->getName().c_str(), nullptr, nullptr);
		if (mWindow == nullptr)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}


		return Engine::onInit(loader);
	}

	bool WindowsEngine::postInit(ResourceLoader* loader)
	{
		mInputSystem->setWindow(mWindow);
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
		mTimeOld = glfwGetTime();
		mTimeNow = 0;
	}

	void WindowsEngine::preUpdate()
	{
		glfwPollEvents();
		mTimeNow = glfwGetTime();
		sDeltaTime = static_cast<float>(mTimeNow - mTimeOld);
		mTimeOld = mTimeNow;
	}

	void WindowsEngine::postUpdate()
	{
		GameObject::clearDeletedGameObjects();
		mIsRunning = !glfwWindowShouldClose(mWindow);
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
