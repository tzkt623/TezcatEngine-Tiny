#include "Scene.h"
#include "../Component/GameObject.h"

#include "../Manager/LightManager.h"
#include "../Manager/CameraManager.h"

#include "../Component/Camera.h"
#include "../Component/Light.h"
#include "../Component/Transform.h"
#include "../Layer/RenderLayer.h"
#include "../Tool/Log.h"

namespace tezcat::Tiny
{
	TINY_RTTI_CPP(Scene);
	
	Scene::Scene(const std::string& name)
		: mName(name)
		, mLightData(new LightData())
		, mCameraData(nullptr)
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::onEnter()
	{
		Log::info(StringTool::stringFormat("Enter Scene: %s", this->getName().c_str()));

		mCameraData = CameraData::create();
		mCameraData->addRef();


		CameraMgr::getInstance()->setCameraData(mCameraData);
		LightMgr::getInstance()->setData(mLightData);
	}

	void Scene::onExit()
	{
		mCameraData->subRef();
		mTransformList.clear();
		mObjectList.clear();
		mLogicList.clear();

		CameraMgr::getInstance()->setCameraData(nullptr);
		Log::info(StringTool::stringFormat("Exit Scene: %s", this->getName().c_str()));
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
		mNewObjectList.push_back(gameObject);
		mObjectList.push_back(gameObject);
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
			auto child = (*it);
			if (child.lock())
			{
				child->update();
				it++;
			}
			else
			{
				it = mTransformList.erase(it);
			}
		}
	}

	TinyVector<GameObject*> Scene::findGameObjects(const std::string& name)
	{
		TinyVector<GameObject*> result;
		for (auto object : mObjectList)
		{
			if (object->getName() == name)
			{
				result.push_back(object);
			}
		}

		return result;
	}

	uint32_t Scene::addUpdateTransform(Transform* transform)
	{
		// 		mUpdateTransformList.emplace_back(transform);
		// 		return mUpdateTransformList.size() - 1;
		return 0;
	}

	void Scene::setUpdateTransform(const uint32_t& index, Transform* transform)
	{
		//mUpdateTransformList[index] = transform;
	}
}
