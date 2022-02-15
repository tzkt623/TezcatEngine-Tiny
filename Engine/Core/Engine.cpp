#include "Engine.h"
#include "Manager/LightManager.h"
#include "Manager/ShaderManager.h"
#include "Manager/SceneManager.h"
#include "Manager/CameraManager.h"
#include "Data/ResourceLoader.h"
#include "Input/InputSystem.h"
#include "Renderer/BaseGraphics.h"
#include "Scene/LayerMask.h"
#include "Scene/GameObject.h"
#include "Manager/VertexGroupManager.h"


namespace tezcat::Tiny::Core
{
	int Engine::ScreenHeight = 0;
	int Engine::ScreenWidth = 0;
	float Engine::DeltaTime = 0;

	Engine::Engine()
		: m_ResourceLoader(nullptr)
		, m_LightManager(new LightManager())
		, m_ShaderManager(new ShaderManager())
		, m_SceneManager(new SceneManager())
		, m_CameraManager(new CameraManager())
		, m_InputSystem(new InputSystem())
		, m_Graphics(nullptr)
		, m_IsRunning(true)
	{
		new VertexGroupManager();
	}

	Engine::~Engine()
	{
		delete m_InputSystem;
		delete m_Graphics;
		delete m_SceneManager;
		delete m_ShaderManager;
		delete m_CameraManager;
		delete m_ResourceLoader;
		delete m_LightManager;
	}

	bool Engine::init(ResourceLoader* loader)
	{
		if (!this->preInit(loader))
		{
			return false;
		}

		if (!this->onInit(loader))
		{
			return false;
		}

		if (!this->postInit(loader))
		{
			return false;
		}

		return true;
	}

	bool Engine::preInit(ResourceLoader* loader)
	{
		m_ResourceLoader = loader;
		m_ResourceLoader->prepareEngine(this);

		ScreenWidth = m_ResourceLoader->getWindowWidth();
		ScreenHeight = m_ResourceLoader->getWindowHeight();

		LayerMask::init();
		return true;
	}

	bool Engine::onInit(ResourceLoader* loader)
	{
		m_Graphics = this->createGraphics();
		GraphicsT::attach(m_Graphics);
		m_Graphics->init(this);

		m_ResourceLoader->prepareResource(this);
		return true;
	}

	bool Engine::postInit(ResourceLoader* loader)
	{
		m_SceneManager->init();
		m_ResourceLoader->prepareScene(this);
		return true;
	}
	void Engine::beforeLoop()
	{

	}

	void Engine::endLoop()
	{

	}

	void Engine::run()
	{
		this->beforeLoop();

		while (m_IsRunning)
		{
			this->preUpdate();
			this->onUpdate();
			this->postUpdate();
		}

		this->endLoop();
	}

	void Engine::preUpdate()
	{

	}

	void Engine::onUpdate()
	{
		m_InputSystem->update();
		m_SceneManager->update();
		m_Graphics->render();
		m_Graphics->swapBuffer();
	}

	void Engine::postUpdate()
	{

	}

	void Engine::stop()
	{
		m_IsRunning = false;
	}
}