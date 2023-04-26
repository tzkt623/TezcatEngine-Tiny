#include "MyController.h"
#include "MyInputer.h"
#include "Scene/MyMainScene.h"

MyController::MyController()
	: mFront(0.0f, 0.0f, -1.0f)
	, mUp(0.0f, 1.0f, 0.0f)
	, mRight(1.0f, 0.0f, 0.0f)
	, mWorldUp(0.0f, 1.0f, 0.0f)
	, mYaw(-90.0f)
	, mPitch(0.0f)
	, mRoll(0.0f)
	, mWorldIndex(-1)
	, mWorld1Gate(nullptr)
	, mWorld2Gate(nullptr)
{

}

MyController::~MyController()
{

}

void MyController::onStart()
{
	this->getTransform()->setDelegateUpdate(std::bind(&MyController::updateVector, this, std::placeholders::_1));
}

void MyController::onEnable()
{
	//	MyInputer::getInstance()->setController(this);
	//	this->startLogic(std::bind(&MyController::warp, this));
}

void MyController::warp()
{
	const float warp_distance = 235.0f;

	if (mWorldIndex == -1)
	{
		mWorldIndex = 1;
		auto objects = this->getGameObject()->getScene()->findGameObjects("World1_Gate");
		mWorld1Gate = objects[0]->getTransform();

		objects = this->getGameObject()->getScene()->findGameObjects("World2_Gate");
		mWorld2Gate = objects[0]->getTransform();
	}

	if (mWorldIndex == 1)
	{
		if (mWorld2Gate != nullptr)
		{
			auto distance = glm::distance(this->getTransform()->getPosition(), mWorld2Gate->getPosition());
			if (distance < warp_distance)
			{
				mWorldIndex = 2;
				CameraMgr::getInstance()->setMainCamera("World2_Camera");

				auto camera1 = CameraMgr::getInstance()->getCamera("World1_Camera");
				camera1->setFrameBuffer(FrameBufferMgr::getInstance()->find("FB_World1"));

				auto camera2 = CameraMgr::getInstance()->getCamera("World2_Camera");
				camera2->setFrameBuffer(nullptr);
			}
		}
	}
	else if (mWorldIndex == 2)
	{
		if (mWorld1Gate != nullptr)
		{
			auto distance = glm::distance(this->getTransform()->getPosition(), mWorld1Gate->getPosition());
			if (distance < warp_distance)
			{
				mWorldIndex = 1;
				CameraMgr::getInstance()->setMainCamera("World1_Camera");

				auto camera1 = CameraMgr::getInstance()->getCamera("World1_Camera");
				camera1->setFrameBuffer(nullptr);

				auto camera2 = CameraMgr::getInstance()->getCamera("World2_Camera");
				camera2->setFrameBuffer(FrameBufferMgr::getInstance()->find("FB_World2"));
			}
		}
	}
}

void MyController::yawPitch(float yaw, float pitch, bool constrainPitch)
{
// 	yaw *= 0.2f;
// 	pitch *= 0.2f;

	mYaw += yaw;
	mPitch += pitch;

	if (mYaw > 360.0f)
	{
		mYaw -= 360.0f;
	}
	else if (mYaw < -360.0f)
	{
		mYaw += 360.0f;
	}

	if (constrainPitch)
	{
		if (mPitch > 89.0f)
		{
			mPitch = 89.0f;
		}

		if (mPitch < -89.0f)
		{
			mPitch = -89.0f;
		}
	}

	this->getTransform()->setRotation(mPitch, mYaw, mRoll);
}

void MyController::roll(float roll)
{
	mRoll = roll;
	mWorldUp = glm::normalize(mWorldUp * glm::angleAxis(glm::radians(mRoll), mFront));
	// 	mRoll += roll;
	// 	if (mRoll > 360.0f)
	// 	{
	// 		mRoll -= 360.0f;
	// 	}
	// 	else if (mRoll < 360.0)
	// 	{
	// 		mRoll += 360.0f;
	// 	}
}

void MyController::updateVector(Transform* transform)
{
	glm::vec3 front;
	front.x = glm::cos(glm::radians(mYaw)) * glm::cos(glm::radians(mPitch));
	front.y = glm::sin(glm::radians(mPitch));
	front.z = glm::sin(glm::radians(mYaw)) * glm::cos(glm::radians(mPitch));
	mFront = glm::normalize(front);

	mRight = glm::normalize(glm::cross(mFront, mWorldUp));
	mUp = glm::normalize(glm::cross(mRight, mFront));

	glm::mat4 m4(1.0f);
	auto& matrix = transform->getModelMatrix();
	auto& position = transform->getPosition();
	matrix = glm::lookAt(position, position + mFront, mUp);


	if (transform->getParent() != nullptr)
	{
		matrix = transform->getParent()->getModelMatrix() * matrix;
	}
}

