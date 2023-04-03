#include "Engine.h"
#include "Manager/LightManager.h"
#include "Manager/ShaderManager.h"
#include "Manager/SceneManager.h"
#include "Manager/CameraManager.h"
#include "Data/ResourceLoader.h"
#include "Input/InputSystem.h"
#include "Renderer/BaseGraphics.h"
#include "Scene/LayerMask.h"
#include "Component/GameObject.h"
#include "Manager/VertexGroupManager.h"
#include "Shader/ShaderParam.h"
#include "Manager/PipelineManager.h"


namespace tezcat::Tiny::Core
{
	int Engine::ScreenHeight = 0;
	int Engine::ScreenWidth = 0;
	float Engine::sDeltaTime = 0;

	Engine::Engine()
		: m_ResourceLoader(nullptr)
		, m_LightManager(new LightManager())
		, m_ShaderManager(new ShaderManager())
		, m_SceneManager(new SceneManager())
		, m_CameraManager(new CameraManager())
		, m_InputSystem(new InputSystem())
		, mGraphics(nullptr)
		, m_IsRunning(true)
	{
		new VertexGroupManager();
	}

	Engine::~Engine()
	{
		delete m_InputSystem;
		delete mGraphics;
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
		ShaderParam::initAllParam(std::bind(&ResourceLoader::initYourShaderParam, m_ResourceLoader));
		m_ResourceLoader->prepareEngine(this);

		ScreenWidth = m_ResourceLoader->getWindowWidth();
		ScreenHeight = m_ResourceLoader->getWindowHeight();

		LayerMask::init();
		return true;
	}

	bool Engine::onInit(ResourceLoader* loader)
	{
		mGraphics = this->createGraphics();
		Graphics::attach(mGraphics);
		mGraphics->init(this);

		m_ResourceLoader->prepareResource(this);
		PipelineManager::loadAllShaderToRenderer(mGraphics);
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
		mGraphics->render();
		mGraphics->swapBuffer();
	}

	void Engine::postUpdate()
	{

	}

	void Engine::stop()
	{
		m_IsRunning = false;
	}
}
