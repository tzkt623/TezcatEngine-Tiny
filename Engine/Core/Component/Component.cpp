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

	void Component::startLogic(const std::function<void()>& logicFunction)
	{
		m_GameObject->getScene()->addLogicFunction(this, logicFunction);
	}

	void Component::stopLogic()
	{
		m_GameObject->getScene()->removeLogicFunction(this);
	}
}

