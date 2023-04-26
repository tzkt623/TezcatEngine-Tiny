#include "Component.h"
#include "../Scene/Scene.h"
#include "../Component/GameObject.h"
#include "Transform.h"

namespace tezcat::Tiny
{
	uint32_t Component::sID = 0;

	Component::Component()
		: mGameObject(nullptr)
		, mEnable(true)
	{

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

	void Component::onDestroy()
	{
		//mGameObject->removeComponent(this);
	}
}

