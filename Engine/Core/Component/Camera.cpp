#include "Camera.h"
#include "Component.h"
#include "Transform.h"

#include "../Manager/CameraManager.h"
#include "GameObject.h"

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
