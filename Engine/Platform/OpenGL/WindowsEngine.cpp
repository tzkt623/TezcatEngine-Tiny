#include "WindowsEngine.h"
#include "Core/EngineIniter.h"
#include "Core/Manager/LightManager.h"
#include "Core/Manager/ShaderManager.h"
#include "Core/Manager/SceneManager.h"
#include "Core/Manager/CameraManager.h"
#include "Core/Input/InputSystem.h"
#include "Core/Renderer/LayerMask.h"
#include "Core/Component/GameObject.h"
#include "Core/GUI/GUI.h"
#include "Core/Tool/Log.h"

#include "../OpenGL/GLGraphics.h"

#include <semaphore>

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

	bool WindowsEngine::preInit()
	{
		return Engine::preInit();
	}

	bool WindowsEngine::onInit()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		mWindow = glfwCreateWindow(ScreenWidth, ScreenHeight, mResourceLoader->getName().c_str(), nullptr, nullptr);
		if (mWindow == nullptr)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}


		return Engine::onInit();
	}

	bool WindowsEngine::postInit()
	{
		mInputSystem->setWindow(mWindow);
		return Engine::postInit();
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
		InputSys::getInstance()->mouseButtonCallBack(button, action, mods);
	}

	void WindowsEngine::mouseCursorPosCallBack(GLFWwindow* window, double xpos, double ypos)
	{
		InputSys::getInstance()->mouseCursorPosCallBack(xpos, ypos);
	}

	void WindowsEngine::mouseScrollPosCallBack(GLFWwindow* window, double xoffset, double yoffset)
	{
		InputSys::getInstance()->mouseScrollPosCallBack(xoffset, yoffset);
	}

	//-----------------------------------------------------------------
	//
	//	WindowsEditor
	//
	bool WindowsEditor::preInit()
	{
		return Engine::preInit();
	}

	bool WindowsEditor::onInit()
	{
		return Engine::onInit();
	}

	bool WindowsEditor::postInit()
	{
		mWindow = ((GLGraphics*)mGraphics)->getGLFWwindow();
		mInputSystem->setWindow(mWindow);
		return Engine::postInit();
	}

	void WindowsEditor::beforeLoop()
	{
		Engine::beforeLoop();
		mTimeOld = glfwGetTime();
		mTimeNow = 0;
	}

	void WindowsEditor::endLoop()
	{
		Engine::endLoop();
		glfwTerminate();
	}

	void WindowsEditor::preUpdate()
	{
		Engine::preUpdate();
		mTimeNow = glfwGetTime();
		sDeltaTime = static_cast<float>(mTimeNow - mTimeOld);
		mTimeOld = mTimeNow;
	}

	void WindowsEditor::onUpdate()
	{
		//mInputSystem->update();
		mSceneManager->update();
	}

	void WindowsEditor::notifyRender()
	{
		mGraphics->render();
		glfwPollEvents();
		glfwSwapBuffers(mWindow);
	}

	void WindowsEditor::postUpdate()
	{
		Engine::postUpdate();
		mIsRunning = !glfwWindowShouldClose(mWindow);
	}

	BaseGraphics* WindowsEditor::createGraphics()
	{
		switch (EngineConfig::getRenderAPI())
		{
		case RenderAPI::OpenGL: return new GLGraphics();
		default:
			throw std::logic_error("RenderAPI must not null");
			break;
		}
	}
}
