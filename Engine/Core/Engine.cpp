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
		: mResourceLoader(nullptr)
		, mLightManager(new LightManager())
		, mShaderManager(new ShaderManager())
		, mSceneManager(new SceneManager())
		, mCameraManager(new CameraManager())
		, mInputSystem(new InputSystem())
		, mGraphics(nullptr)
		, mIsRunning(true)
	{
		new VertexGroupManager();
	}

	Engine::~Engine()
	{
		delete mInputSystem;
		delete mGraphics;
		delete mSceneManager;
		delete mShaderManager;
		delete mCameraManager;
		delete mResourceLoader;
		delete mLightManager;
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
		mResourceLoader = loader;
		ShaderParam::initAllParam(std::bind(&ResourceLoader::initYourShaderParam, mResourceLoader));
		mResourceLoader->prepareEngine(this);

		ScreenWidth = mResourceLoader->getWindowWidth();
		ScreenHeight = mResourceLoader->getWindowHeight();

		LayerMask::init();
		return true;
	}

	bool Engine::onInit(ResourceLoader* loader)
	{
		mGraphics = this->createGraphics();
		Graphics::attach(mGraphics);
		mGraphics->init(this);

		mResourceLoader->prepareResource(this);
		return true;
	}

	bool Engine::postInit(ResourceLoader* loader)
	{
		mSceneManager->init();
		mResourceLoader->prepareScene(this);
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

		while (mIsRunning)
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
		mInputSystem->update();
		mSceneManager->update();
		mGraphics->render();
		mGraphics->swapBuffer();
	}

	void Engine::postUpdate()
	{

	}

	void Engine::stop()
	{
		mIsRunning = false;
	}
}
