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

#include "Core/Component/FlyController.h"
#include "Core/Component/Transform.h"
#include "Core/Input/InputSystem.h"
#include "Core/Input/Inputer.h"
#include "Core/Tool/Singleton.h"

namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(FlyController, ComponentAutoID<FlyController>)


	/// <summary>
	/// 这是个标准位置坐标,永远不会改变
	/// 主要是与forward向量一起来计算真正的up
	/// 如果forward向量不与WorldUp垂直
	/// 那么真正的up向量就是斜的
	/// </summary>
	const float3 FlyController::sWorldUp = float3(0.0f, 1.0f, 0.0f);

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
		InputSys::getInstance()->currentInputer()->setController(this);
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
		float3 front;
		front.x = glm::cos(glm::radians(mPitch)) * glm::cos(glm::radians(mYaw));
		front.y = glm::sin(glm::radians(mPitch));
		front.z = glm::cos(glm::radians(mPitch)) * glm::sin(glm::radians(mYaw));
		mFront = glm::normalize(front);

		mRight = glm::normalize(glm::cross(mFront, sWorldUp));
		mUp = glm::normalize(glm::cross(mRight, mFront));

		auto& matrix = transform->getModelMatrix();
		auto& position = transform->getPosition();
		matrix = glm::lookAt(position, position + mFront, mUp);
		//matrix = glm::lookAt(float3(0.0f, 0.0f, 10.0f), float3(0.0f), float3(0.0f, 1.0f, 0.0f));

		if (transform->getParent() != nullptr)
		{
			matrix = transform->getParent()->getModelMatrix() * matrix;
		}
	}

	float3 FlyController::getForward()
	{
		return this->getTransform()->getForward();
	}

	float3 FlyController::getUp()
	{
		return this->getTransform()->getUp();
	}

	float3 FlyController::getRight()
	{
		return this->getTransform()->getRight();
	}

	void FlyController::translate(const float3& offset)
	{
		this->getTransform()->translate(offset);
	}

}
