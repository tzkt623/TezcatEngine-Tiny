#include "SceneManager.h"
#include "../Scene/Scene.h"
#include "Utility/Utility.h"
#include "../Statistic.h"

namespace tezcat::Tiny::Core
{
	SceneManager::SceneManager()
	{
		SG<SceneManager>::attach(this);
	}

	SceneManager::~SceneManager()
	{

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
			mScenes.push(it->second);
			it->second->onEnter();
		}
	}

	void SceneManager::pushScene(Scene* scene)
	{
		if (!mScenes.empty())
		{
			if (mScenes.top() == scene)
			{
				return;
			}

			mScenes.top()->onPause();
		}

		mScenes.push(scene);
		scene->onEnter();
	}

	void SceneManager::popScene()
	{
		mScenes.top()->onExit();
		mScenes.pop();
		mScenes.top()->onResume();
	}

	void SceneManager::prepareScene(Scene* scene)
	{
		mSceneWithName.emplace(scene->getName(), scene);
	}

	bool SceneManager::update()
	{
		TINY_PROFILER_TIMER_OUT(Statistic::LogicTime);
		if (mScenes.empty())
		{
			return false;
		}

		mScenes.top()->update();
		return true;
	}
}
