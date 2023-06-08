#include "SceneManager.h"
#include "../Scene/Scene.h"
#include "../Tool/Tool.h"
#include "../Profiler.h"
#include "../Event/EngineEvent.h"

namespace tezcat::Tiny
{
	SceneManager::SceneManager()
		: mNeedSwith(false)
	{
		SG<SceneManager>::attach(this);

		EngineEvent::get()->addListener(EngineEventID::EE_PopScene
			, this
			, [this](const EventData& data)
			{
				this->popScene();
			});

		EngineEvent::get()->addListener(EngineEventID::EE_PushScene
			, this
			, [this](const EventData& data)
			{
				this->pushScene((Scene*)data.userData);
			});
	}

	SceneManager::~SceneManager()
	{
		EngineEvent::get()->removeListener(this);
	}

	void SceneManager::init()
	{

	}

	void SceneManager::pushScene(const std::string& name)
	{
		if (!mScenes.empty())
		{
			mScenes.top()->onPause();
		}

		auto it = mSceneWithName.find(name);
		if (it != mSceneWithName.end())
		{
			this->pushScene(it->second);
		}
	}

	void SceneManager::pushScene(Scene* scene)
	{
		mCDMs.emplace_back(CMD::Push, scene);
	}

	void SceneManager::popScene()
	{
		if (mScenes.empty())
		{
			return;
		}
		mCDMs.emplace_back(CMD::Pop, mScenes.top());
	}

	void SceneManager::prepareScene(Scene* scene)
	{
		mSceneWithName.emplace(scene->getName(), scene);
	}

	bool SceneManager::update()
	{
		TINY_PROFILER_TIMER_OUT(Profiler::LogicTime);

		while (!mCDMs.empty())
		{
			auto cmd = mCDMs.front();
			mCDMs.pop_front();
			switch (cmd.cmd)
			{
			case CMD::Pop:
			{
				EngineEvent::get()->dispatch({ EngineEventID::EE_OnPopScene });
				mScenes.top()->onExit();
				mScenes.pop();

				if (!mScenes.empty())
				{
					mScenes.top()->onResume();
				}
				break;
			}
			case CMD::Push:
			{
				if (!mScenes.empty())
				{
					if (mScenes.top() == cmd.scene)
					{
						break;
					}

					mScenes.top()->onPause();
				}

				mScenes.push(cmd.scene);
				EngineEvent::get()->dispatch({ EngineEventID::EE_OnPushScene });
				cmd.scene->onEnter();
				break;
			}
			default:
				break;
			}
		}

		if (mScenes.empty())
		{
			return false;
		}

		mScenes.top()->update();
		return true;
	}

	void SceneManager::switchScene(Scene* scene)
	{
		if (!mScenes.empty())
		{
			mScenes.top()->onExit();
			mScenes.pop();
			EngineEvent::get()->dispatch(EventData{ EngineEventID::EE_OnPopScene });
		}

		mScenes.push(scene);
		EngineEvent::get()->dispatch(EventData{ EngineEventID::EE_OnPushScene });
		scene->onEnter();
	}

}
