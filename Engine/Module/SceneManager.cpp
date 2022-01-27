#include "SceneManager.h"
#include "Scene.h"

namespace tezcat::Tiny::Module
{
	IMPLEMENT_DELEGATE_INSTANCE(SceneManager);

	SceneManager::SceneManager()
	{
		REGISTER_DELEGATE_INSTANCE(SceneManager);
	}

	SceneManager::~SceneManager()
	{
		
	}

	void SceneManager::init()
	{
	}

	void SceneManager::pushScene(const std::string& name)
	{
		if (!m_Scenes.empty())
		{
			m_Scenes.top()->onPause();
		}

		auto it = m_SceneWithName.find(name);
		if (it != m_SceneWithName.end())
		{
			it->second->onEnter();
			m_Scenes.push(it->second);
		}
	}

	void SceneManager::popScene()
	{
		m_Scenes.top()->onExit();
		m_Scenes.pop();
		m_Scenes.top()->onResume();
	}

	void SceneManager::prepareScene(Scene* scene)
	{
		m_SceneWithName.emplace(scene->getName(), scene);
	}

	void SceneManager::update()
	{
		m_Scenes.top()->update();
	}
}