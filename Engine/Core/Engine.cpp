#include <iostream>
#include <vector>
#include <initializer_list>

#include "Engine.h"
#include "ShaderBuilder.h"
#include "Shader.h"
#include "MeshData.h"
#include "Renderer.h"
#include "VAO.h"
#include "Pass_VAO.h"
#include "RenderObject.h"
#include "SceneManager.h"
#include "ShaderManager.h"
#include "CameraManager.h"
#include "InputSystem.h"
#include "ResourceLoader.h"
#include "Tools.h"


namespace tezcat::Tiny::Core
{
	int Engine::ScreenHeight = 0;
	int Engine::ScreenWidth = 0;
	float Engine::DeltaTime = 0;

	Engine::Engine() :
		m_Window(nullptr),
		m_Renderer(new Renderer()),
		m_ShaderManager(new ShaderManager()),
		m_SceneManager(new Module::SceneManager()),
		m_CameraManager(new Module::CameraManager()),
		m_InputSystem(new Module::InputSystem()),
		m_ResourceLoader(nullptr)
	{

	}

	Engine::~Engine()
	{
		glfwTerminate();

		delete m_Renderer;
		delete m_SceneManager;
		delete m_ShaderManager;
		delete m_CameraManager;
	}

	int Engine::init(Module::ResourceLoader* loader)
	{
		m_ResourceLoader = loader;
		m_ResourceLoader->prepareEngine(this);

		ScreenWidth = m_ResourceLoader->getWindowWidth();
		ScreenHeight = m_ResourceLoader->getWindowHeight();

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow(ScreenWidth, ScreenHeight, m_ResourceLoader->getName().c_str(), nullptr, nullptr);
		if (m_Window == nullptr)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(1);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}

		m_Renderer->init(this);

		m_ResourceLoader->prepareResource(this);
		m_ResourceLoader->prepareScene(this);

		this->initInputSystem();
		m_SceneManager->init();


		return 0;
	}

	void Engine::initInputSystem()
	{
		m_InputSystem->setWindow(m_Window);
// 		glfwSetMouseButtonCallback(m_Window, Engine::mouseButtonCallBack);
// 		glfwSetCursorPosCallback(m_Window, Engine::mouseCursorPosCallBack);
// 		glfwSetScrollCallback(m_Window, Engine::mouseScrollPosCallBack);
	}

	void Engine::run()
	{
		double old_time = glfwGetTime();
		double now_time = 0;

		while (!glfwWindowShouldClose(m_Window))
		{
			glfwPollEvents();

			now_time = glfwGetTime();
			DeltaTime = static_cast<float>(now_time - old_time);
			old_time = now_time;

			m_InputSystem->processInput();

			m_SceneManager->update();

			m_Renderer->render();

			glfwSwapBuffers(m_Window);
		}
	}

	void Engine::close()
	{

	}

	void Engine::mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods)
	{
		Engine::getInstance()->getInputSystem()->mouseButtonCallBack(button, action, mods);
	}

	void Engine::mouseCursorPosCallBack(GLFWwindow* window, double xpos, double ypos)
	{
		Engine::getInstance()->getInputSystem()->mouseCursorPosCallBack(xpos, ypos);
	}

	void Engine::mouseScrollPosCallBack(GLFWwindow* window, double xoffset, double yoffset)
	{
		Engine::getInstance()->getInputSystem()->mouseScrollPosCallBack(xoffset, yoffset);
	}
}