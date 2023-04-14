#include "Scene.h"
#include "../Component/GameObject.h"

#include "../Manager/LightManager.h"
#include "../Manager/CameraManager.h"

#include "../Component/Camera.h"
#include "../Component/Light.h"
#include "../Component/Transform.h"
#include "../Layer/RenderLayer.h"

namespace tezcat::Tiny::Core
{
	Scene::Scene(const std::string& name)
		: mName(name)
		, mLightData(new LightData())
		, mCameraData(new CameraData())
	{

	}

	Scene::~Scene()
	{

	}

	void Scene::onEnter()
	{
		CameraMgr::getInstance()->setCameraData(mCameraData);
		LightMgr::getInstance()->setData(mLightData);
	}

	void Scene::onExit()
	{

	}

	void Scene::onPause()
	{

	}

	void Scene::onResume()
	{
		CameraMgr::getInstance()->setCameraData(mCameraData);
		LightMgr::getInstance()->setData(mLightData);
	}

	void Scene::addGameObject(GameObject* gameObject)
	{
		mNewObjectList.emplace_back(gameObject);
		mObjectList.emplace_back(gameObject);
	}

	void Scene::addTransform(Transform* transform)
	{
		mTransformList.emplace_back(transform);
	}

	void Scene::addCamera(Camera* camera)
	{
		CameraMgr::getInstance()->addCamera(camera);
	}

	void Scene::addLogicFunction(void* gameObject, const std::function<void()>& function)
	{
		mLogicList[gameObject] = function;
	}

	void Scene::removeLogicFunction(void* gameObject)
	{
		mLogicList.erase(gameObject);
	}

	void Scene::addNewObject(GameObject* gameObject)
	{
		mNewObjectList.emplace_back(gameObject);
	}

	void Scene::update()
	{
		//#InitNewObjects
		if (!mNewObjectList.empty())
		{
			for (auto go : mNewObjectList)
			{
				go->enterScene(this);
			}
			mNewObjectList.clear();
		}

		//#SceneUpdate
		if (!mLogicList.empty())
		{
			for (const auto& pair : mLogicList)
			{
				pair.second();
			}
		}

		//#TransformUpdate
		auto it = mTransformList.begin();
		auto end = mTransformList.end();
		while (it != end)
		{
			auto go = (*it)->getGameObject();
			if (go->needDelete())
			{
				it = mTransformList.erase(it);
				end = mTransformList.end();
				go->exitScene();
				go->close();
				delete go;
			}
			else
			{
				(*it)->update();
				it++;
			}
		}

		//#CameraCulling
// 		auto it_camera = m_CameraData->m_CameraList.begin();
// 		auto end_camera = m_CameraData->m_CameraList.end();
// 		while (it_camera != end_camera)
// 		{
// 			auto c = (*it_camera);
// 			if (c->isEnable())
// 			{
// 				const auto& cull_list = c->getCullLayerList();
// 				for (auto& index : cull_list)
// 				{
// 					m_RenderLayerList[index]->testWithCamera(c, m_LightLayerList[index]);
// 				}
// 
// 				it_camera++;
// 			}
// 			else
// 			{
// 				it_camera = m_CameraData->m_CameraList.erase(it_camera);
// 				end_camera = m_CameraData->m_CameraList.end();
// 			}
// 		}
	}

	std::vector<GameObject*> Scene::findGameObjects(const std::string& name)
	{
		std::vector<GameObject*> result;
		for (auto object : mObjectList)
		{
			if (object->getName() == name)
			{
				result.push_back(object);
			}
		}

		return result;
	}

}
