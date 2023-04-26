#include "GameObject.h"
#include "../Scene/Scene.h"
#include "../Manager/SceneManager.h"
#include "../Tool/Tool.h"


namespace tezcat::Tiny
{
	TINY_RTTI_CPP(GameObject)

	std::stack<int> GameObject::sUIDPool;
	std::list<GameObject*> GameObject::sDeleteObjectList;

	GameObject::GameObject()
		: GameObject("GameObject")
	{

	}

	GameObject::GameObject(const std::string& name)
		: mIsLogic(false)
		, mScene(SceneMgr::getInstance()->getCurrentScene())
		, mNeedDelete(false)
		, mLayerMaskIndex(0)
		, mUID(-1)
		, mTransform(nullptr)
		, mName(name)
	{
		if (sUIDPool.empty())
		{
			mUID = IDGenerator<GameObject, int>::generate();
		}
		else
		{
			mUID = sUIDPool.top();
			sUIDPool.pop();
		}

		mScene->addGameObject(this);
	}

	GameObject::~GameObject()
	{
		if (mTransform->getParent() != nullptr)
		{

		}

		auto children = this->getTransform()->getChildren();
		if (children)
		{
			for (auto& child : *children)
			{
				child->getGameObject()->subRef();
			}
		}

		mComponentList.clear();
		sUIDPool.push(mUID);
		mScene = nullptr;
	}

	void GameObject::enterScene(Scene* scene)
	{
		for (auto com : mComponentList)
		{
			com->onStart();
		}

		auto children = this->getTransform()->getChildren();
		if (children)
		{
			for (auto& child : *children)
			{
				child->getGameObject()->enterScene(mScene);
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

	void GameObject::destory(GameObject* go)
	{
		go->mNeedDelete = true;
		sDeleteObjectList.emplace_back(go);
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

	void GameObject::clearDeletedGameObjects()
	{
		if (sDeleteObjectList.empty())
		{
			return;
		}

		for (auto go : sDeleteObjectList)
		{
			//delete go;
		}
		sDeleteObjectList.clear();
	}
}
