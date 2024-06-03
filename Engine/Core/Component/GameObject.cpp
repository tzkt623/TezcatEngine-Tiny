#include "GameObject.h"
#include "../Scene/Scene.h"
#include "../Manager/SceneManager.h"
#include "../Tool/Tool.h"


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

	}

	void GameObject::init()
	{
		mUID = SceneManager::getCurrentScene()->addGameObject(this);
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
		auto children = this->getTransform()->getChildren();
		if (children)
		{
			for (auto& child : *children)
			{
				child->getGameObject()->deleteObject();
			}
		}

		SceneManager::getCurrentScene()->removeGameObject(this);

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
		for (uint32 i = 0; i < mComponentList.size(); i++)
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

	//-------------------------------------------
	//
	//	GameObjectData
	//
	TINY_OBJECT_CPP(GameObjectData, TinyObject)
	GameObjectData::GameObjectData()
	{

	}

	GameObjectData::~GameObjectData()
	{
		for (auto go : mArray)
		{
			if (go)
			{
				go->deleteObject();
			}
		}
		mArray.clear();
	}

	int32 GameObjectData::addGameObject(GameObject* gameObject)
	{
		gameObject->saveObject();
		if (mFreeIDQueue.empty())
		{
			int32 id = mArray.size();
			mArray.push_back(gameObject);
			return id;
		}
		else
		{
			int32 id = mFreeIDQueue.front();
			mFreeIDQueue.pop();
			mArray[id] = gameObject;
			return id;
		}
	}

	void GameObjectData::removeGameObject(GameObject* gameObject)
	{
		int32 id = gameObject->getUID();
		mFreeIDQueue.push(id);
		mArray[id] = nullptr;
	}
}
