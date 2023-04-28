#include "Engine.h"
#include "Manager/LightManager.h"
#include "Manager/ShaderManager.h"
#include "Manager/SceneManager.h"
#include "Manager/CameraManager.h"
#include "Manager/VertexManager.h"
#include "Manager/PipelineManager.h"
#include "Manager/FrameBufferManager.h"
#include "Manager/TextureManager.h"
#include "Manager/PipelineManager.h"

#include "Data/ResourceLoader.h"
#include "Input/InputSystem.h"
#include "Renderer/BaseGraphics.h"
#include "Renderer/LayerMask.h"
#include "Component/GameObject.h"
#include "Shader/ShaderParam.h"

#include "Tool/Tool.h"
#include "Event/EngineEvent.h"

#include "Base/TinyGC.h"

namespace tezcat::Tiny
{
	int Engine::ScreenHeight = 0;
	int Engine::ScreenWidth = 0;
	float Engine::sDeltaTime = 0;

	Engine::Engine()
		: mResourceLoader(nullptr)
		, mLightManager(nullptr)
		, mShaderManager(nullptr)
		, mSceneManager(nullptr)
		, mCameraManager(nullptr)
		, mFrameBufferManager(nullptr)
		, mTextureManager(nullptr)
		, mPipelineManager(nullptr)
		, mVertexManager(nullptr)
		, mInputSystem(nullptr)
		, mGraphics(nullptr)
		, mIsRunning(true)
	{

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
		EngineEvent::get()->init(EngineEventID::EE_IDCount);
		mResourceLoader = loader;

		mLightManager = new LightManager();
		mShaderManager = new ShaderManager();
		mSceneManager = new SceneManager();
		mCameraManager = new CameraManager();
		mFrameBufferManager = new FrameBufferManager();
		mTextureManager = new TextureManager();
		mPipelineManager = new PipelineManager();
		mVertexManager = new VertexManager();
		mInputSystem = new InputSystem();

		if (!this->preInit())
		{
			return false;
		}

		if (!this->onInit())
		{
			return false;
		}

		if (!this->postInit())
		{
			return false;
		}

		return true;
	}

	bool Engine::preInit()
	{
		mResourceLoader->prepareEngine(this);
		ScreenWidth = mResourceLoader->getWindowWidth();
		ScreenHeight = mResourceLoader->getWindowHeight();

		ShaderParam::initAllParam(std::bind(&ResourceLoader::initYourShaderParam, mResourceLoader));

		LayerMask::init();
		FileTool::init(mResourceLoader->getResourceFolderName());
		return true;
	}

	bool Engine::onInit()
	{
		mGraphics = this->createGraphics();
		mGraphics->init(this);

		mResourceLoader->prepareResource(this);
		return true;
	}

	bool Engine::postInit()
	{
		mSceneManager->init();
		mResourceLoader->prepareGame(this);
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
	}

	void Engine::postUpdate()
	{
		TinyGC::update();
	}

	void Engine::stop()
	{
		mIsRunning = false;
	}
}
