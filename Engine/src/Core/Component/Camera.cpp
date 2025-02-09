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

#include "Core/Component/Camera.h"
#include "Core/Component/Component.h"
#include "Core/Component/Transform.h"

#include "Core/Manager/CameraManager.h"
#include "Core/Component/GameObject.h"

namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(Camera, ComponentT<Camera>)

	Camera::Camera(bool mainCamera)
		: mCameraAgent(CameraAgent::create())
		, mIsMain(mainCamera)
	{
		mCameraAgent->saveObject();
	}

	Camera::Camera()
		: Camera(true)
	{

	}

	Camera::~Camera()
	{
		mCameraAgent->deleteObject();
	}

	void Camera::onStart()
	{
		mCameraAgent->setTransform(mGameObject->getTransform());
	}

	void Camera::onEnable()
	{
		CameraManager::addCamera(this);
	}

	void Camera::onDisable()
	{

	}

	void Camera::setMain()
	{
		mIsMain = true;
		CameraManager::setMainCamera(this);
	}
}
