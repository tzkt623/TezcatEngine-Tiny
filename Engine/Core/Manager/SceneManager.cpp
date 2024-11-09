#include "SceneManager.h"
#include "../Scene/Scene.h"
#include "../Tool/Tool.h"
#include "../Profiler.h"
#include "../Event/EngineEvent.h"

namespace tezcat::Tiny
{
	std::deque<tezcat::Tiny::SceneManager::CMDData> SceneManager::mCDMs;
	std::unordered_map<std::string_view, Scene*> SceneManager::sSceneUMap;
	std::stack<Scene*> SceneManager::sSceneArray;

	void SceneManager::init()
	{
		EngineEvent::getInstance()->addListener(EngineEventID::EE_PopScene
			, &sSceneArray
			, [](const EventData& data)
			{
				popScene();
			});

		EngineEvent::getInstance()->addListener(EngineEventID::EE_PushScene
			, &sSceneArray
			, [](const EventData& data)
			{
				pushScene((Scene*)data.userData);
			});
	}

	void SceneManager::pushScene(const std::string& name)
	{
		if (!sSceneArray.empty())
		{
			sSceneArray.top()->onPause();
		}

		auto it = sSceneUMap.find(name);
		if (it != sSceneUMap.end())
		{
			pushScene(it->second);
		}
	}

	void SceneManager::pushScene(Scene* scene)
	{
		mCDMs.emplace_back(CMD::Push, scene);
	}

	void SceneManager::popScene()
	{
		if (sSceneArray.empty())
		{
			return;
		}

		mCDMs.emplace_back(CMD::Pop, sSceneArray.top());
	}

	void SceneManager::prepareScene(Scene* scene)
	{
		auto result = sSceneUMap.try_emplace(scene->getName(), nullptr);
		if (result.second)
		{
			result.first->second = scene;
			scene->saveObject();
		}
		else
		{
			TINY_THROW("Fatal Error!");
		}
	}

	bool SceneManager::update()
	{
		TINY_PROFILER_TIMER_OUT(Profiler::LogicTime);

		while (!mCDMs.empty())
		{
			auto& cmd = mCDMs.front();
			mCDMs.pop_front();
			switch (cmd.cmd)
			{
			case CMD::Pop:
			{
				EngineEvent::getInstance()->dispatch({ EngineEventID::EE_OnPopScene });
				sSceneArray.top()->onExit();
				sSceneArray.pop();

				if (!sSceneArray.empty())
				{
					sSceneArray.top()->onResume();
				}
				break;
			}
			case CMD::Push:
			{
				if (!sSceneArray.empty())
				{
					if (sSceneArray.top() == cmd.scene)
					{
						break;
					}

					sSceneArray.top()->onPause();
				}

				sSceneArray.push(cmd.scene);
				EngineEvent::getInstance()->dispatch({ EngineEventID::EE_OnPushScene });
				cmd.scene->onEnter();
				break;
			}
			default:
				break;
			}
		}

		if (sSceneArray.empty())
		{
			return false;
		}

		sSceneArray.top()->update();
		return true;
	}

	void SceneManager::switchScene(Scene* scene)
	{
		if (!sSceneArray.empty())
		{
			sSceneArray.top()->onExit();
			sSceneArray.pop();
			EngineEvent::getInstance()->dispatch(EventData{ EngineEventID::EE_OnPopScene });
		}

		sSceneArray.push(scene);
		EngineEvent::getInstance()->dispatch(EventData{ EngineEventID::EE_OnPushScene });
		scene->onEnter();
	}



}
