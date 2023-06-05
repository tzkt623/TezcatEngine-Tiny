#include "Scene.h"
#include "../Component/GameObject.h"

#include "../Manager/LightManager.h"
#include "../Manager/CameraManager.h"

#include "../Component/Camera.h"
#include "../Component/Light.h"
#include "../Component/Transform.h"
#include "../Renderer/RenderLayer.h"
#include "../Tool/Log.h"

namespace tezcat::Tiny
{
	TINY_RTTI_CPP(Scene);

	Scene::Scene(const std::string& name)
		: mName(name)
		, mLightData(nullptr)
		, mCameraData(nullptr)
	{

	}

	Scene::~Scene()
	{

	}

	void Scene::onEnter()
	{
		Log_Info(StringTool::stringFormat("Enter Scene: %s", this->getName().c_str()));

		mCameraData = CameraData::create();
		mCameraData->addRef();

		mLightData = LightData::create();
		mLightData->addRef();


		CameraMgr::getInstance()->setCameraData(mCameraData);
		LightMgr::getInstance()->setData(mLightData);
	}

	void Scene::onExit()
	{
		mLightData->subRef();
		mCameraData->subRef();

		mLogicList.clear();
		mTransformList.clear();

		for (auto obj : mObjectList)
		{
			if (auto ptr = obj.lock())
			{
				ptr->subRef();
			}
		}
		mObjectList.clear();

		CameraMgr::getInstance()->setCameraData(nullptr);
		LightMgr::getInstance()->setData(nullptr);

		Log_Info(StringTool::stringFormat("Exit Scene: %s", this->getName().c_str()));
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
		// 		mNewObjectList.push_back(gameObject);
		// 		mObjectList.push_back(gameObject);
	}

	void Scene::addNewTransform(Transform* transform)
	{
		mNewTransformList.push_back(transform);
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
		if (!mNewTransformList.empty())
		{
			for (auto transform : mNewTransformList)
			{
				if (transform->getParent() == nullptr)
				{
					transform->getGameObject()->enterScene(this);
					mTransformList.emplace_back(transform);
					mObjectList.emplace_back(transform->getGameObject());
				}
			}

			mNewTransformList.clear();
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

	std::vector<TinyWeakRef<GameObject>> Scene::findGameObjects(const std::string& name)
	{
		std::vector<TinyWeakRef<GameObject>> result;
		auto it = mObjectList.begin();
		while (it != mObjectList.end())
		{
			auto object = *it;
			if (auto ptr = object.lock())
			{
				if (object->getName() == name)
				{
					result.emplace_back(object);
				}
				++it;
			}
			else
			{
				mObjectList.erase(it);
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
