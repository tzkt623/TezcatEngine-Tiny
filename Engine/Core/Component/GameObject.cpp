#include "GameObject.h"
#include "../Scene/Scene.h"
#include "../Manager/SceneManager.h"
#include "Utility/Utility.h"


namespace tezcat::Tiny::Core
{
	std::stack<int> GameObject::sUIDPool;
	std::list<GameObject*> GameObject::sDeleteObjectList;

	GameObject::GameObject()
		: GameObject("GameObject")
	{

	}

	GameObject::GameObject(const std::string& name)
		: mName(name)
		, mIsLogic(false)
		, mScene(SceneMgr::getInstance()->getCurrentScene())
		, mNeedDelete(false)
		, mLayerMaskIndex(0)
		, mUID(-1)
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

		//		this->addComponent<Transform>();

		//处理第一个component
		//创建GameObject并不会把对象加入场景树中
		//需要创建Transform,并且parent==null才会加入场景中
// 		auto tran = new Transform();
// 		tran->setGameObject(this);
// 		mComponentList.push_back(tran);

		mScene->addGameObject(this);
	}

	GameObject::~GameObject()
	{
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
		for (auto child : children)
		{
			child->getGameObject()->enterScene(mScene);
		}
	}

	void GameObject::exitScene()
	{
		auto children = this->getTransform()->getChildren();
		for (auto child : children)
		{
			child->getGameObject()->exitScene();
		}

		for (auto com : mComponentList)
		{
			com->onDisable();
		}

		mScene = nullptr;
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

		mComponentList.emplace_back(component);
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
			delete go;
		}
		sDeleteObjectList.clear();
	}

	void GameObject::close()
	{
		auto children = this->getTransform()->getChildren();
		for (auto child : children)
		{
			child->getGameObject()->close();
		}

		for (auto com : mComponentList)
		{
			com->onDisable();
		}

		mScene = nullptr;
	}
}
