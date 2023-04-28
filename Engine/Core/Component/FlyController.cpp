#include "FlyController.h"
#include "Transform.h"

namespace tezcat::Tiny
{
	TINY_RTTI_CPP(FlyController)


	/// <summary>
	/// 这是个标准位置坐标,永远不会改变
	/// 主要是与forward向量一起来计算真正的up
	/// 如果forward向量不与WorldUp垂直
	/// 那么真正的up向量就是斜的
	/// </summary>
	const glm::vec3 FlyController::sWorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	FlyController::FlyController()
		: mFront(0.0f, 0.0f, -1.0f)
		, mUp(0.0f, 1.0f, 0.0f)
		, mRight(1.0f, 0.0f, 0.0f)
		, mYaw(0.0f)
		, mPitch(0.0f)
		, mRoll(0.0f)
	{

	}

	FlyController::~FlyController()
	{

	}

	void FlyController::onStart()
	{

	}

	void FlyController::onEnable()
	{

	}

	void FlyController::yawPitch(float yaw, float pitch, bool constrainPitch)
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

		this->getTransform()->setRotation(mPitch, mYaw);
	}

	void FlyController::roll(float roll)
	{
		mRoll += roll;
		if (mRoll > 360.0f)
		{
			mRoll -= 360.0f;
		}
		else if (mRoll < -360.0f)
		{
			mRoll += 360.0f;
		}
		this->getTransform()->setRotation(mPitch, mYaw, mRoll);
	}

	void FlyController::updateVector(Transform* transform)
	{
		glm::vec3 front;
		front.x = glm::cos(glm::radians(mPitch)) * glm::cos(glm::radians(mYaw));
		front.y = glm::sin(glm::radians(mPitch));
		front.z = glm::cos(glm::radians(mPitch)) * glm::sin(glm::radians(mYaw));
		mFront = glm::normalize(front);

		mRight = glm::normalize(glm::cross(mFront, sWorldUp));
		mUp = glm::normalize(glm::cross(mRight, mFront));

		auto& matrix = transform->getModelMatrix();
		auto& position = transform->getPosition();
		matrix = glm::lookAt(position, position + mFront, mUp);
		//matrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		if (transform->getParent() != nullptr)
		{
			matrix = transform->getParent()->getModelMatrix() * matrix;
		}
	}

	glm::vec3 FlyController::getForward()
	{
		return this->getTransform()->getForward();
	}

	glm::vec3 FlyController::getUp()
	{
		return this->getTransform()->getUp();
	}

	glm::vec3 FlyController::getRight()
	{
		return this->getTransform()->getRight();
	}

	void FlyController::translate(const glm::vec3& offset)
	{
		this->getTransform()->translate(offset);
	}

}
