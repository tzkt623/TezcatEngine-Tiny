#include "GameObject.h"
#include "Scene.h"
#include "../Manager/SceneManager.h"
#include "Utility/Tools.h"


namespace tezcat::Tiny::Core
{
	std::stack<int> GameObject::s_UIDPool;
	std::list<GameObject*> GameObject::s_DeleteObjectList;

	GameObject::GameObject()
		: GameObject("GameObject")
	{

	}

	GameObject::GameObject(const std::string& name)
		: m_Name(name)
		, m_IsLogic(false)
		, m_Scene(nullptr)
		, m_NeedDelete(false)
		, m_LayerMaskIndex(0)
		, m_UID(-1)
	{
		if (s_UIDPool.empty())
		{
			m_UID = IDGenerator<GameObject, int>::generate();
		}
		else
		{
			m_UID = s_UIDPool.top();
			s_UIDPool.pop();
		}

		this->addComponent<Transform>();
	}

	GameObject::~GameObject()
	{
		m_ComponentList.clear();
		s_UIDPool.push(m_UID);
		m_Scene = nullptr;
	}

	void GameObject::enterScene(Scene* scene)
	{
		m_Scene = scene;
		for (auto const& com : m_ComponentList)
		{
			com->onStart();
		}

		auto chldren = this->getTransform()->getChildren();
		if (!chldren.empty())
		{
			for (auto child : chldren)
			{
				child->enterScene(scene);
			}
		}
	}

	void GameObject::exitScene()
	{
		auto chldren = this->getTransform()->getChildren();
		if (!chldren.empty())
		{
			for (auto child : chldren)
			{
				child->exitScene();
			}
		}

		for (auto const& com : m_ComponentList)
		{
			com->onDisable();
		}

		m_Scene = nullptr;
	}


	void GameObject::destory(GameObject* go)
	{
		go->m_NeedDelete = true;
		s_DeleteObjectList.push_back(go);
	}

	void GameObject::addComponent(Component* component)
	{
		m_ComponentList.emplace_back(component);

		component->setGameObject(this);
		component->setTransform((Transform*)m_ComponentList[0].get());
		component->onEnable();
	}

	void GameObject::clearDeletedGameObjects()
	{
		if (s_DeleteObjectList.empty())
		{
			return;
		}

		for (auto go : s_DeleteObjectList)
		{
			delete go;
		}
		s_DeleteObjectList.clear();
	}

}