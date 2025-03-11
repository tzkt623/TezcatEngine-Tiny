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

#include "Core/Scene.h"
#include "Core/Component/GameObject.h"

#include "Core/Manager/CameraManager.h"
#include "Core/Manager/SceneManager.h"
#include "Core/Manager/LightingManager.h"
#include "Core/Manager/ResourceManager.h"
#include "Core/Manager/GameObjectManager.h"

#include "Core/Component/Camera.h"
#include "Core/Component/Light.h"
#include "Core/Component/Transform.h"

#include "Core/Renderer/RenderObjectCache.h"

#include "Core/Event/EngineEvent.h"

#include "Core/Debug/Debug.h"


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
		TINY_LOG_INFO(std::format("Enter Scene: {}", mName));

		EngineEvent::getInstance()->addListener(EngineEventID::EE_LoadModel, this,
			[](const EventData& evt)
			{
				std::string* path = (std::string*)evt.userData;
				Model* model = Model::create();
				if (model->load(*path))
				{
					model->setPath(*path);
					model->generate();
				}
			});

		mCameraData = CameraData::create();
		mCameraData->saveObject();

		mLightData = LightData::create();
		mLightData->saveObject();

		CameraManager::setData(mCameraData);
		LightingManager::setLightData(mLightData);
		GameObjectManager::enterScene();
	}

	void Scene::onExit()
	{
		for (auto& tran : mTransformList)
		{
			tran->getGameObject()->deleteObject();
		}

		mLightData->deleteObject();
		mCameraData->deleteObject();

		mLogicList.clear();
		mTransformList.clear();

		CameraManager::setData(nullptr);
		LightingManager::setLightData(nullptr);
		GameObjectManager::exitScene();

		EngineEvent::getInstance()->removeListener(this);

		TINY_LOG_INFO(std::format("Exit Scene: {}", mName));
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
			for (auto& go : mNewGameObjectList)
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

	void Scene::addGameObject(GameObject* gameObject)
	{
		mNewGameObjectList.push_back(gameObject);
	}
}
