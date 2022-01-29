#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "CameraManager.h"

namespace tezcat::Tiny::Module
{
	Scene::Scene(const std::string& name) :
		m_Name(name)
	{

	}

	Scene::~Scene()
	{

	}

	void Scene::onEnter()
	{
		CameraManager::getInstance()->setSharedCameras(&m_CameraWithName);
	}

	void Scene::onExit()
	{

	}

	void Scene::onPause()
	{

	}

	void Scene::onResume()
	{

	}

	void Scene::addGameObject(GameObject* go)
	{
		go->sceneEnter(this);
		m_ObjectList.push_back(go);
		m_NewEnterObjectList.push_back(go);
	}

	void Scene::addCamera(Camera* camera)
	{
		auto it = m_CameraWithName.find(camera->getName());
		if (it == m_CameraWithName.end())
		{
			m_CameraWithName.emplace(camera->getName(), camera);
			Core::Engine::getInstance()->getCameraManager()->addCamera(camera);
		}
	}

	void Scene::addLogicFunction(GameObject* gameObject, const std::function<void()>& function)
	{
		m_LogicList[gameObject] = function;
	}

	void Scene::removeLogicFunction(GameObject* gameObject)
	{
		m_LogicList.erase(gameObject);
	}

	void Scene::foreachChild(
		const std::function<void()>& begin,
		const std::function<void(GameObject*)>& get,
		const std::function<void()>& end,
		const std::function<void()>& nochildren)
	{
		if (!m_ObjectList.empty())
		{
			for (auto child : m_ObjectList)
			{
				get(child);
				child->foreachChild(begin, get, end, nochildren);
			}
		}
		else
		{
			nochildren();
		}
	}

	void Scene::update()
	{
		// 		if (!m_NewEnterObjectList.empty())
		// 		{
		// 			for (auto go : m_NewEnterObjectList)
		// 			{
		// 				go->update();
		// 			}
		// 			m_NewEnterObjectList.clear();
		// 		}
		// 

		for (auto go : m_ObjectList)
		{
			go->update();
		}

		if (!m_LogicList.empty())
		{
			for (auto const& pair : m_LogicList)
			{
				pair.second();
			}
		}
	}
}

