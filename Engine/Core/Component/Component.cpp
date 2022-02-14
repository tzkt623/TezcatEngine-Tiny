#include "Component.h"
#include "../Scene/Scene.h"
#include "../Scene/GameObject.h"
#include "Transform.h"

namespace tezcat::Tiny::Core
{
	template class ComponentT<Transform>;

	Component::Component()
		: m_Transform(nullptr)
		, m_GameObject(nullptr)
		, m_Enable(true)
	{
		this->onAwake();
	}

	Component::~Component()
	{
		this->onDestroy();
		m_Transform = nullptr;
		m_GameObject = nullptr;
	}

	template<class Com>
	Com* Component::getComponentInChildren()
	{
		auto result = m_GameObject->getComponent<Com>();
		if (result != nullptr)
		{
			return result;
		}

		auto children = m_Transform->getChildren();
		for (auto child : children)
		{
			auto result = child->getComponentInChildren<Com>();
			if (result != nullptr)
			{
				return result;
			}
		}

		return nullptr;
	}

	template<class Com>
	Com* Component::getComponent()
	{
		return m_GameObject->getComponent<Com>();
	}

	void Component::startLogic(const std::function<void()>& logicFunction)
	{
		m_GameObject->getScene()->addLogicFunction(this, logicFunction);
	}

	void Component::stopLogic()
	{
		m_GameObject->getScene()->removeLogicFunction(this);
	}
}

