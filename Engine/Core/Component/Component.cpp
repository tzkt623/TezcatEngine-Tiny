#include "Component.h"
#include "../Scene/Scene.h"
#include "../Component/GameObject.h"
#include "Transform.h"

namespace tezcat::Tiny::Core
{
	uint32_t Component::sID = 0;

	Component::Component()
		: mGameObject(nullptr)
		, mEnable(true)
	{
		this->onAwake();
	}

	Component::~Component()
	{
		this->onDestroy();
		mGameObject = nullptr;
	}

	void Component::startLogic(const std::function<void()>& logicFunction)
	{
		mGameObject->getScene()->addLogicFunction(this, logicFunction);
	}

	void Component::stopLogic()
	{
		mGameObject->getScene()->removeLogicFunction(this);
	}

	Transform* Component::getTransform()
	{
		return mGameObject->mTransform;
	}

	void Component::close()
	{
		mGameObject->removeComponent(this);
		delete this;
	}
}

