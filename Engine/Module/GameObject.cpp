#include "GameObject.h"
#include "RenderObject.h"
#include "Transform.h"
#include "Material.h"
#include "Engine.h"
#include "Renderer.h"
#include "Scene.h"
#include "Mesh.h"
#include <functional>


namespace tezcat::Tiny::Module
{
	GameObject::GameObject() :
		GameObject("GameObject")
	{

	}

	GameObject::GameObject(const std::string& name) :
		m_Name(name),
		m_Transform(new Transform()),
		m_RenderObject(nullptr),
		m_Material(nullptr),
		m_IsLogic(false),
		m_Scene(nullptr),
		m_UserObject(nullptr)
	{

	}

	GameObject::~GameObject()
	{
		for (auto child : m_Children)
		{
			delete child;
		}

		m_Children.clear();

		delete m_Material;
		delete m_UserObject;

		m_Material = nullptr;
		m_RenderObject = nullptr;
		m_Scene = nullptr;
	}

	void GameObject::addChild(GameObject* gameObject)
	{
		gameObject->getTransform()->setParent(m_Transform);
		m_Children.push_back(gameObject);
	}

	void GameObject::onApply()
	{
		if (m_RenderObject != nullptr)
		{
			m_RenderObject->setMasterObject(this);
			m_RenderObject->setMaterial(m_Material);
			m_RenderObject->apply();
			Core::Engine::getInstance()->getRenderer()->dispatch(this);
		}
	}

	void GameObject::sceneEnter(Scene* scene)
	{
		m_Scene = scene;

		if (!m_Children.empty())
		{
			for (auto child : m_Children)
			{
				child->sceneEnter(scene);
			}
		}
	}

	void GameObject::sceneExit(Scene* scene)
	{

	}

	void GameObject::setMaterial(Material* material)
	{
		m_Material = material;
	}

	void GameObject::apply()
	{
		this->onApply();

		if (!m_Children.empty())
		{
			for (auto child : m_Children)
			{
				child->apply();
			}
		}
	}

	void GameObject::startLogic()
	{
		if (!m_IsLogic)
		{
			m_IsLogic = true;
			m_Scene->addLogicFunction(this, std::bind(&GameObject::logic, this));
		}
	}

	void GameObject::stopLogic()
	{
		if (m_IsLogic)
		{
			m_IsLogic = false;
			m_Scene->removeLogicFunction(this);
		}
	}

	void GameObject::update()
	{
		m_Transform->update();
		if (!m_Children.empty())
		{
			for (auto child : m_Children)
			{
				child->update();
			}
		}
	}

	void GameObject::logic()
	{
	}

	void GameObject::setMesh(Core::Mesh* mesh)
	{
		m_RenderObject = new Core::ROMesh(mesh);
	}

	void GameObject::setRenderObject(Core::RenderObject* renderObject)
	{
		m_RenderObject = renderObject;
	}

	void GameObject::foreachChild(
		const std::function<void()>& begin,
		const std::function<void(GameObject*)>& get,
		const std::function<void()>& end,
		const std::function<void()>& nochildren)
	{
		if (m_Children.empty())
		{
			nochildren();
			return;
		}

		begin();
		for (auto child : m_Children)
		{
			get(child);
			child->foreachChild(begin, get, end, nochildren);
		}
		end();
	}

}