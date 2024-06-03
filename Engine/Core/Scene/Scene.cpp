#include "Scene.h"
#include "../Component/GameObject.h"

#include "../Manager/CameraManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/LightingManager.h"

#include "../Component/Camera.h"
#include "../Component/Light.h"
#include "../Component/Transform.h"
#include "../Renderer/RenderObjectCache.h"
#include "../Tool/Log.h"

namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(Scene, TinyObject)

	Scene::Scene(std::string name)
		: mName(std::move(name))
		, mLightData(nullptr)
		, mCameraData(nullptr)
		, mGameObjectData(nullptr)
	{

	}

	Scene::~Scene()
	{

	}

	void Scene::onEnter()
	{
		Log_Info(StringTool::stringFormat("Enter Scene: %s", mName.c_str()));

		mCameraData = CameraData::create();
		mCameraData->saveObject();

		mLightData = LightData::create();
		mLightData->saveObject();

		mGameObjectData = GameObjectData::create();
		mGameObjectData->saveObject();

		CameraManager::setData(mCameraData);
		LightingManager::setLightData(mLightData);
	}

	void Scene::onExit()
	{
		mLightData->deleteObject();
		mCameraData->deleteObject();
		mGameObjectData->deleteObject();

		mLogicList.clear();
		mTransformList.clear();

		CameraManager::setData(nullptr);
		LightingManager::setLightData(nullptr);

		Log_Info(StringTool::stringFormat("Exit Scene: %s", mName.c_str()));
	}

	void Scene::onPause()
	{
		CameraManager::setData(nullptr);
		LightingManager::setLightData(nullptr);
	}

	void Scene::onResume()
	{
		CameraManager::setData(mCameraData);
		LightingManager::setLightData(mLightData);
	}

	void Scene::addTransform(Transform* transform)
	{
		mTransformList.emplace_back(transform);
	}

	void Scene::addLogicFunction(void* gameObject, const std::function<void()>& function)
	{
		mLogicList[gameObject] = function;
	}

	void Scene::removeLogicFunction(void* gameObject)
	{
		mLogicList.erase(gameObject);
	}

	/// <summary>
	/// 退出后这里可能有问题
	/// </summary>
	void Scene::update()
	{
		//#InitNewObjects
		if (!mNewGameObjectList.empty())
		{
			for (auto go : mNewGameObjectList)
			{
				go->enterScene(this);
				if (go->getTransform()->getParent() == nullptr)
				{
					mTransformList.push_back(go->getTransform());
				}
			}

			mNewGameObjectList.clear();
		}

		//#TransformUpdate
		auto it = mTransformList.begin();
		auto end = mTransformList.end();
		while (it != end)
		{
			auto& child = *it;
			if (child.lock())
			{
				child->update();
				it++;
			}
			else
			{
				it = mTransformList.erase(it);
				end = mTransformList.end();
			}
		}

		//#SceneUpdate
		if (!mLogicList.empty())
		{
			for (auto& pair : mLogicList)
			{
				pair.second();
			}
		}
	}

	TinyWeakRef<Transform> Scene::findTransform(const std::string& name)
	{
		auto it = mTransformList.begin();
		while (it != mTransformList.end())
		{
			auto& ptr = *it;
			if (ptr->getGameObject()->getName() == name)
			{
				return ptr;
			}
			++it;
		}

		return nullptr;
	}

	std::vector<TinyWeakRef<Transform>> Scene::findTransforms(const std::string& name)
	{
		std::vector<TinyWeakRef<Transform>> result;
		auto it = mTransformList.begin();
		while (it != mTransformList.end())
		{
			auto& ptr = *it;
			if (ptr->getGameObject()->getName() == name)
			{
				result.emplace_back(ptr);
			}
			++it;
		}

		return result;
	}

	void Scene::open()
	{
		SceneManager::switchScene(this);
	}

	void Scene::push()
	{
		SceneManager::pushScene(this);
	}

	void Scene::prepare()
	{
		SceneManager::prepareScene(this);
	}

	int32 Scene::addGameObject(GameObject* gameObject)
	{
		mNewGameObjectList.push_back(gameObject);
		return mGameObjectData->addGameObject(gameObject);
	}

	void Scene::removeGameObject(GameObject* gameObject)
	{
		mGameObjectData->removeGameObject(gameObject);
	}

}
