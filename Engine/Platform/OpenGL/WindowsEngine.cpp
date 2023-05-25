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
#include "Core/Tool/Log.h"

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
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, mGLMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, mGLMinor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		mWindow = glfwCreateWindow(ScreenWidth, ScreenHeight, mResourceLoader->getName().c_str(), nullptr, nullptr);
		if (mWindow == nullptr)
		{
			//std::cout << "Failed to create GLFW window" << std::endl;
			Log_Error("Failed to create GLFW window");
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(mWindow);
		glfwSwapInterval(mResourceLoader->isEnabelVsync() ? 1 : 0);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			Log_Error("Failed to initialize GLAD");
			TinyThrow_Logic("Failed to initialize GLAD");
		}

		GLint max;
		glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &max);
		//std::cout << "GL_MAX_UNIFORM_LOCATIONS: " << max << std::endl;
		Log_Engine(StringTool::stringFormat("GL_MAX_UNIFORM_LOCATIONS: %d", max));

		auto get_default_color_buffer = [&](GLint id)
		{
			switch (id)
			{
			case GL_NONE:
				//std::cout << "Default ColorBuffer: GL_NONE" << std::endl;
				Log_Engine("Default ColorBuffer: GL_NONE");
				break;
			case GL_FRONT_LEFT:
				//std::cout << "Default ColorBuffer: GL_FRONT_LEFT" << std::endl;
				Log_Engine("Default ColorBuffer: GL_FRONT_LEFT");
				break;
			case GL_FRONT_RIGHT:
				//std::cout << "Default ColorBuffer: GL_FRONT_RIGHT" << std::endl;
				Log_Engine("Default ColorBuffer: GL_FRONT_RIGHT");
				break;
			case GL_BACK_LEFT:
				//std::cout << "Default ColorBuffer: GL_BACK_LEFT" << std::endl;
				Log_Engine("Default ColorBuffer: GL_BACK_LEFT");
				break;
			case GL_BACK_RIGHT:
				//std::cout << "Default ColorBuffer: GL_BACK_RIGHT" << std::endl;
				Log_Engine("Default ColorBuffer: GL_BACK_RIGHT");
				break;
			case GL_FRONT:
				//std::cout << "Default ColorBuffer: GL_FRONT" << std::endl;
				Log_Engine("Default ColorBuffer: GL_FRONT");
				break;
			case GL_BACK:
				//std::cout << "Default ColorBuffer: GL_BACK" << std::endl;
				Log_Engine("Default ColorBuffer: GL_BACK");
				break;
			case GL_LEFT:
				//std::cout << "Default ColorBuffer: GL_LEFT" << std::endl;
				Log_Engine("Default ColorBuffer: GL_LEFT");
				break;
			case GL_RIGHT:
				//std::cout << "Default ColorBuffer: GL_RIGHT" << std::endl;
				Log_Engine("Default ColorBuffer: GL_RIGHT");
				break;
			case GL_FRONT_AND_BACK:
				//std::cout << "Default ColorBuffer: GL_FRONT_AND_BACK" << std::endl;
				Log_Engine("Default ColorBuffer: GL_FRONT_AND_BACK");
				break;
			default:
				break;
			}
		};

		glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &max);
		//std::cout << "GL_MAX_UNIFORM_LOCATIONS: " << max << std::endl;
		Log_Engine(StringTool::stringFormat("GL_MAX_UNIFORM_LOCATIONS: %d", max));

		glGetIntegerv(GL_DRAW_BUFFER, &max);
		get_default_color_buffer(max);
		glGetIntegerv(GL_READ_BUFFER, &max);
		get_default_color_buffer(max);

		mGUI = new GUI();
		mGUI->init(mWindow);

		return Engine::onInit();
	}

	bool WindowsEditor::postInit()
	{
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
		glfwPollEvents();
		mTimeNow = glfwGetTime();
		sDeltaTime = static_cast<float>(mTimeNow - mTimeOld);
		mTimeOld = mTimeNow;
	}

	void WindowsEditor::onUpdate()
	{
		//mInputSystem->update();
		mSceneManager->update(mGraphics);
		mGraphics->render();
		mGUI->render();
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

	void WindowsEditor::setGLVersion(int major, int minor)
	{
		mGLMajor = major;
		mGLMinor = minor;
	}

	GUI* WindowsEditor::getGUI()
	{
		return mGUI;
	}

}
