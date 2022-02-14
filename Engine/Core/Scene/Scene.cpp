#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"

#include "../Manager/LightManager.h"
#include "../Manager/CameraManager.h"

#include "../Component/Camera.h"
#include "../Component/Transform.h"

namespace tezcat::Tiny::Core
{
	Scene::Scene(const std::string& name)
		: m_Name(name)
		, m_LightData(new LightData())
	{

	}

	Scene::~Scene()
	{

	}

	void Scene::onEnter()
	{
		SG<CameraManager>::getInstance()->setCurrentSceneCameras(m_CameraWithName);
	}

	void Scene::onExit()
	{

	}

	void Scene::onPause()
	{

	}

	void Scene::onResume()
	{
		SG<CameraManager>::getInstance()->setCurrentSceneCameras(m_CameraWithName);
	}

	void Scene::addTransform(Transform* transform)
	{
		transform->enterScene(this);
		m_TransformList.push_back(transform);
	}

	void Scene::addCamera(Camera* camera)
	{
		auto it = m_CameraWithName.find(camera->getGameObject()->getName());
		if (it == m_CameraWithName.end())
		{
			m_CameraWithName.emplace(camera->getGameObject()->getName(), camera);
			CameraMgr::getInstance()->addCamera(camera);
		}
	}

	void Scene::addLogicFunction(void* gameObject, const std::function<void()>& function)
	{
		m_LogicList[gameObject] = function;
	}

	void Scene::removeLogicFunction(void* gameObject)
	{
		m_LogicList.erase(gameObject);
	}

	void Scene::update()
	{
		if (!m_NewObjectList.empty())
		{
			for (auto go : m_NewObjectList)
			{
				go->enterScene(this);
			}
			m_NewObjectList.clear();
		}

		//#SceneUpdate
		if (!m_LogicList.empty())
		{
			for (auto const& pair : m_LogicList)
			{
				pair.second();
			}
		}

		auto it = m_TransformList.begin();
		while (it != m_TransformList.end())
		{
			auto go = (*it)->getGameObject();
			if (go->needDelete())
			{
				go->exitScene();
				it = m_TransformList.erase(it);
			}
			else
			{
				(*it++)->update();
			}
		}

		CameraMgr::getInstance()->render();
	}

	void Scene::addGameObject(GameObject* gameObject)
	{
		m_TransformList.emplace_back(gameObject->getTransform());
		m_NewObjectList.push_back(gameObject);
	}
}