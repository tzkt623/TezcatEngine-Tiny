#include "GameObject.h"
#include "../Scene/Scene.h"
#include "../Manager/SceneManager.h"
#include "../Tool/Tool.h"


namespace tezcat::Tiny
{
	TINY_RTTI_CPP(GameObject);

	GameObject::GameObject()
		: GameObject("GameObject")
	{

	}

	GameObject::GameObject(const std::string& name)
		: mIsLogic(false)
		, mScene(nullptr)
		, mLayerMaskIndex(0)
		, mUID(-1)
		, mName(name)
	{
		mUID = GameObjectPool::addGameObject(this);
	}

	GameObject::~GameObject()
	{
		auto children = this->getTransform()->getChildren();
		if (children)
		{
			for (auto& child : *children)
			{
				child->getGameObject()->subRef();
			}
		}

		mComponentList.clear();
		mScene = nullptr;

		GameObjectPool::removeGameObject(this);
	}

	void GameObject::enterScene(Scene* scene)
	{
		mScene = scene;
		for (auto com : mComponentList)
		{
			com->onStart();
		}

		auto children = this->getTransform()->getChildren();
		if (children)
		{
			for (auto& child : *children)
			{
				child->getGameObject()->enterScene(scene);
			}
		}
	}

	void GameObject::exitScene()
	{
		for (auto com : mComponentList)
		{
			com->onDisable();
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
			mComponentList[i]->onComponentAdded(component);
		}

		mComponentList.push_back(component);
		component->onEnable();
	}

	//-------------------------------------------------
	//
	//
	//
	TinyVector<GameObject*> GameObjectPool::sPool;
	std::deque<int> GameObjectPool::sFreeObjects;

	int GameObjectPool::addGameObject(GameObject* gameObject)
	{
		int uid;
		if (sFreeObjects.empty())
		{
			uid = sPool.size();
			sPool.push_back(gameObject);
		}
		else
		{
			uid = sFreeObjects.front();
			sFreeObjects.pop_front();
			sPool[uid] = gameObject;
			gameObject->addRef();
		}

		return uid;
	}

	void GameObjectPool::removeGameObject(GameObject* gameObject)
	{
		auto uid = gameObject->getUID();
		sPool[uid] = nullptr;
		sFreeObjects.push_back(uid);
	}

}
