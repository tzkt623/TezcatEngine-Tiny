/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Core/Engine.h"
#include "Core/EngineIniter.h"

#include "Core/Manager/SceneManager.h"
#include "Core/Manager/VertexBufferManager.h"
#include "Core/Manager/ResourceManager.h"

#include "Core/Renderer/BaseGraphics.h"
#include "Core/Renderer/LayerMask.h"
#include "Core/Renderer/Pipeline.h"

#include "Core/Component/GameObject.h"
#include "Core/Shader/ShaderParam.h"

#include "Core/Tool/Tool.h"
#include "Core/Event/EngineEvent.h"
#include "Core/Input/InputSystem.h"

#include "Core/Base/TinyGC.h"

namespace tezcat::Tiny
{
	bool Engine::mIsRunning = true;

	std::mutex Engine::sMainMutex;
	std::condition_variable Engine::sCVLogicThread;
	std::condition_variable Engine::sCVRenderThread;
	std::atomic<bool> Engine::allowLogic(false);
	std::atomic<bool> Engine::allowRender(false);

	std::binary_semaphore Engine::sSemRenderThread(0);
	std::binary_semaphore Engine::sSemLogicThread(0);

	bool Engine::sMultiThread = false;

	Engine::Engine()
		: mResourceLoader(nullptr)
		, mInputSystem(nullptr)
		, mGraphics(nullptr)
	{

	}

	Engine::~Engine()
	{
		TinyString::deleteAllString();
		UniformID::deleteAllString();

		delete mInputSystem;
		delete mGraphics;
		delete mResourceLoader;
	}

	bool Engine::init(EngineIniter* loader)
	{
		EngineEvent::getInstance()->init(EngineEventID::EE_IDCount);
		mResourceLoader = loader;

		mPipeline = new PipelineBuildin();
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

		ShaderParam::initAllParam(std::bind(&EngineIniter::initYourShaderParam, mResourceLoader));

		LayerMask::init();
		ResourceManager::init();
		FileTool::init();
		VertexBufferManager::init();
		return true;
	}

	bool Engine::onInit()
	{
		if (sMultiThread)
		{
			std::atomic<bool> render_thread_init(false);
			mRenderThread = std::thread([this, &render_thread_init]()
			{
				{
					//std::unique_lock lock(sMainMutex);

					mGraphics = this->createGraphics();
					mGraphics->init(this);
					render_thread_init.store(true);
					sSemLogicThread.release();

					//lock.unlock();
					//sCVLogicThread.notify_one();
				}

				allowRender.store(false);

				while (mIsRunning)
				{
					//std::cout << "Render wait signal......\n";
// 					std::unique_lock lock(sMainMutex);
// 					sCVRenderThread.wait(lock, []() { return allowRender.load(); });
					sSemRenderThread.acquire();
					//std::cout << "Render running......\n";

					//mGraphics->render();
					mPipeline->render();

					//allowRender.store(false);
					//allowLogic.store(true);
					//lock.unlock();
					//sCVLogicThread.notify_one();

					sSemLogicThread.release();
				}
			});


			mRenderThread.detach();
			sSemLogicThread.acquire();

			/*
			{
				std::unique_lock lock(sMainMutex);
				sCVLogicThread.wait(lock, [this, &render_thread_init]() { return render_thread_init.load(); });
				std::cout << "wait render thread complete" << std::endl;
			}
			*/
		}
		else
		{
			mGraphics = this->createGraphics();
			mGraphics->init(this);
			mRenderThreadInited = true;
		}

		mResourceLoader->prepareResource(this);
		return true;
	}

	bool Engine::postInit()
	{
		SceneManager::init();
		mResourceLoader->prepareGame(this);
		return true;
	}

	void Engine::beforeLoop()
	{
		allowLogic.store(true);
		sSemLogicThread.release();
	}

	void Engine::endLoop()
	{

	}

	void Engine::run()
	{
		this->beforeLoop();

		while (mIsRunning)
		{
			if (sMultiThread)
			{
				//std::cout << "Logic wait signal......\n";
				//std::unique_lock lock(sMainMutex);
				//sCVLogicThread.wait(lock, []() { return allowLogic.load(); });
				sSemLogicThread.acquire();
				//std::cout << "Logic running......\n";
			}
			//在多线程模式下
			//需要延迟一帧删除数据
			//但是如果所有渲染数据都是拷贝的
			//则可以在帧最后删除
			TinyGC::update();

			this->preUpdate();
			this->onUpdate();
			this->postUpdate();

			this->onRender();
		}

		this->endLoop();
	}

	void Engine::preUpdate()
	{

	}

	void Engine::onUpdate()
	{
		mInputSystem->update();
		SceneManager::update();
	}

	void Engine::onRender()
	{
		if (sMultiThread)
		{
			this->notifyRenderThread();
		}
		else
		{
			this->notifyRender();
		}
	}

	void Engine::postUpdate()
	{

	}

	void Engine::stop()
	{
		mIsRunning = false;
	}

	void Engine::notifyRender()
	{
		/*
		* 1.场景先更新完所有数据
		* 2.玩家相机遍历所有Mesh生成渲染命令
		* 3.管线对命令进行渲染
		*/

		mPipeline->render();
		//mGraphics->render();
	}

	void Engine::notifyRenderThread()
	{
		//allowLogic.store(false);
		//allowRender.store(true);
		//sCVRenderThread.notify_one();
		sSemRenderThread.release();
	}
}
