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

		auto it = m_SceneWithName.find(name);
		if (it != m_SceneWithName.end())
		{
			mScenes.push(it->second);
			it->second->onEnter();
		}
	}

	void SceneManager::popScene()
	{
		mScenes.top()->onExit();
		mScenes.pop();
		mScenes.top()->onResume();
	}

	void SceneManager::prepareScene(Scene* scene)
	{
		m_SceneWithName.emplace(scene->getName(), scene);
	}

	void SceneManager::update()
	{
		TINY_PROFILER_TIMER_OUT(Statistic::LogicTime);
		mScenes.top()->update();
	}
}
