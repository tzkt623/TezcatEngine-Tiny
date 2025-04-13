/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Core/Component/Component.h"
#include "Core/Component/GameObject.h"
#include "Core/Component/Transform.h"

#include "Core/Scene.h"
#include "Core/Manager/SceneManager.h"

namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(Component, TinyObject)

	uint32_t Component::sID = 1;

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

	std::string Component::getMemoryInfo()
	{
		if (mGameObject)
		{
			return TINY_OBJECT_MEMORY_INFO_WTIH_NAME(mGameObject->getEngineName());
		}

		return this->getClassName();
	}

}

