#include "Component.h"
#include "../Scene/Scene.h"
#include "../Manager/SceneManager.h"
#include "../Component/GameObject.h"
#include "Transform.h"

namespace tezcat::Tiny
{
	TINY_RTTI_CPP(Component);
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
		SceneMgr::getInstance()->getCurrentScene()->addLogicFunction(this, logicFunction);
	}

	void Component::stopLogic()
	{
		SceneMgr::getInstance()->getCurrentScene()->removeLogicFunction(this);
	}

	Transform* Component::getTransform()
	{
		return mGameObject->getTransform();
	}

	void Component::onDestroy()
	{
		//mGameObject->removeComponent(this);
	}
}

