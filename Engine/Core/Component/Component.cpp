#include "Component.h"
#include "../Scene/Scene.h"
#include "../Manager/SceneManager.h"
#include "../Component/GameObject.h"
#include "Transform.h"

namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(Component, TinyObject)

	uint32 Component::sID = 0;

	Component::Component()
		: mGameObject(nullptr)
		, mEnable(true)
	{

	}

	Component::~Component()
	{
		mGameObject = nullptr;
	}

	void Component::startLogic(const std::function<void()>& logicFunction)
	{
		SceneManager::getCurrentScene()->addLogicFunction(this, logicFunction);
	}

	void Component::stopLogic()
	{
		SceneManager::getCurrentScene()->removeLogicFunction(this);
	}

	Transform* Component::getTransform()
	{
		return mGameObject->getTransform();
	}

	void Component::onClose()
	{
		mGameObject->removeComponent(this);

		mEnable = false;
		this->onDisable();
		this->onDestroy();
	}

	void Component::setEnable(bool val)
	{
		if (mEnable == val)
		{
			return;
		}

		if (mEnable)
		{
			this->onEnable();
		}
		else
		{
			this->onDisable();
		}
	}
}

