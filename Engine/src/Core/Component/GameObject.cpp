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

#include "Core/Component/GameObject.h"
#include "Core/Scene.h"
#include "Core/Manager/SceneManager.h"
#include "Core/Manager/GameObjectManager.h"
#include "Core/Tool/Tool.h"


namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(GameObject, TinyObject)

	GameObject::GameObject()
		: GameObject("GameObject")
	{
	}

	GameObject::GameObject(std::string name)
		: mIsLogic(false)
		, mScene(nullptr)
		, mLayerMaskIndex(0)
		, mUID(-1)
		, mName(std::move(name))
	{
		mEngineName = mName;
	}

	void GameObject::init()
	{
		mUID = GameObjectManager::addGameObject(this);
		SceneManager::getCurrentScene()->addGameObject(this);
	}

	GameObject::~GameObject()
	{
	}

	void GameObject::enterScene(Scene* scene)
	{
		mScene = scene;
		for (auto com : mComponentList)
		{
			if (com)
			{
				com->onStart();
			}
		}

		//auto children = this->getTransform()->getChildren();
		//if (children)
		//{
		//	for (auto& child : *children)
		//	{
		//		child->getGameObject()->enterScene(scene);
		//	}
		//}
	}

	void GameObject::exitScene()
	{
		for (auto com : mComponentList)
		{
			if (com)
			{
				com->onDisable();
			}
		}

		auto children = this->getTransform()->getChildren();
		if (children)
		{
			for (auto& child : *children)
			{
				child->getGameObject()->exitScene();
			}
		}

		mScene = nullptr;
	}


	void GameObject::swapTransform()
	{
		if (mComponentList.size() > 1)
		{
			auto temp = mComponentList[0];
			mComponentList[0] = mComponentList[mComponentList.size() - 1];
			mComponentList[mComponentList.size() - 1] = temp;
		}
	}

	void GameObject::addComponent(Component* component)
	{
		component->setGameObject(this);

		for (int i = 1; i < mComponentList.size(); i++)
		{
			if (mComponentList[i])
			{
				mComponentList[i]->onComponentAdded(component);
			}
		}

		auto id = component->getComponentTypeID();
		if (mComponentList.size() <= id)
		{
			mComponentList.resize(id + 1);
		}

		mComponentList[id] = component;
		component->saveObject();
		component->onEnable();
	}

	void GameObject::onClose()
	{
		GameObjectManager::removeGameObject(this);

		auto children = this->getTransform()->getChildren();
		if (children)
		{
			for (auto& child : *children)
			{
				child->getGameObject()->deleteObject();
			}
		}

		for (auto com : mComponentList)
		{
			if (com)
			{
				com->deleteObject();
			}
		}
		mComponentList.clear();

		mScene = nullptr;
		mUID = -1;
	}

	void GameObject::removeComponent(Component* com)
	{
		for (uint32_t i = 0; i < mComponentList.size(); i++)
		{		
			if (auto ptr = mComponentList[i])
			{
				if (ptr == com)
				{
					mComponentList[i] = nullptr;
				}
				else
				{
					ptr->onComponentRemoved(com);
				}
			}
		}
	}

	std::string GameObject::getMemoryInfo()
	{
		return TINY_OBJECT_MEMORY_INFO();
	}

}
