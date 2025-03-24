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

#include "Core/Manager/SceneManager.h"

#include "Core/Scene.h"
#include "Core/Debug/Debug.h"


#include "Core/Tool/Tool.h"
#include "Core/Event/EngineEvent.h"

namespace tezcat::Tiny
{
	std::deque<tezcat::Tiny::SceneManager::CMDData> SceneManager::mCDMs;
	std::stack<Scene*> SceneManager::sSceneArray;

	std::unordered_map<std::string_view, Scene*> SceneManager::sPreparedSceneUMap;
	std::vector<Scene*> SceneManager::sPreparedSceneArray;

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

		auto it = sPreparedSceneUMap.find(name);
		if (it != sPreparedSceneUMap.end())
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
		auto it = sPreparedSceneUMap.find(scene->getName());
		if (it == sPreparedSceneUMap.end())
		{
			scene->saveObject();
			sPreparedSceneUMap.emplace(scene->getName(), scene);
			sPreparedSceneArray.push_back(scene);
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
