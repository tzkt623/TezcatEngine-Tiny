﻿/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

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


#include "Core/Component/Transform.h"
#include "Core/Component/GameObject.h"

#include "Core/Manager/SceneManager.h"
#include "Core/Tool/Tool.h"

#include "Core/Scene.h"

namespace tezcat::Tiny
{
	TINY_OBJECT_CPP_TEMPLATE(Transform, ComponentCustomID, Transform, 0);

	static float4x4 WORLD_MATRIX(1.0f);
	const float3 Transform::XAxis(1.0f, 0.0f, 0.0f);
	const float3 Transform::YAxis(0.0f, 1.0f, 0.0f);
	const float3 Transform::ZAxis(0.0f, 0.0f, 1.0f);

	Transform::Transform()
		: Transform(nullptr)
	{

	}

	Transform::Transform(Transform* parent)
		: mIsDirty(true)
		, mLocalPosition(0.0f)
		, mLocalRotation(0.0f)
		, mLocalScale(1.0f)
		, mLocalRotationQ()
		, mWorldPosition(0.0f)
		, mWorldRotation(0.0f)
		, mWorldScale(0.0f)
		, mWorldRotationQ()
		, mModelMatrix(1.0f)
		, mParent(parent)
		, mIndex(0)
		, mDelegateUpdate(TINY_BIND_THIS(Transform::updateMatrix))
		, mChildren(nullptr)
	{

	}

	Transform::~Transform()
	{
		mParent = nullptr;
		delete mChildren;
	}

	void Transform::onStart()
	{

	}

	void Transform::onEnable()
	{

	}

	void Transform::onDisable()
	{

	}

	void Transform::setParent(Transform* parent)
	{
		if (parent == mParent)
		{
			return;
		}

		mIsDirty = true;
		bool is_running = mGameObject->getScene() != nullptr;
		//如果已经在运行了
		if (is_running)
		{
			//如果父节点不为空
			if (mParent != nullptr)
			{
				mParent->removeChild(this);
			}
			else
			{
				mGameObject->getScene()->getTransformList().remove_if([this](TinyWeakRef<Transform>& ptr)
				{
					return ptr.get() == this;
				});
			}

			mParent = parent;
			if (mParent == nullptr)
			{
				mIndex = 0;
				SceneManager::getCurrentScene()->addTransform(this);
			}
			else
			{
				mIndex = mParent->mIndex + 1;
				mParent->addChild(this);
			}

			this->decomposeTransform();
		}
		//如果是新节点准备加入
		else
		{
			//如果父节点不为空
			if (mParent != nullptr)
			{
				//从父节点中删除自己
				mParent->removeChild(this);
			}

			mParent = parent;
			if (mParent == nullptr)
			{
				mIndex = 0;
			}
			else
			{
				mIndex = mParent->mIndex + 1;
				mParent->addChild(this);
			}
		}
	}

	void Transform::clampRotation()
	{
		if (mLocalRotation.x > 360.0f)
		{
			mLocalRotation.x -= 360.0f;
		}
		else if (mLocalRotation.x < -360.0f)
		{
			mLocalRotation.x += 360.0f;
		}

		if (mLocalRotation.y > 360.0f)
		{
			mLocalRotation.y -= 360.0f;
		}
		else if (mLocalRotation.y < -360.0f)
		{
			mLocalRotation.y += 360.0f;
		}

		if (mLocalRotation.z > 360.0f)
		{
			mLocalRotation.z -= 360.0f;
		}
		else if (mLocalRotation.z < -360.0f)
		{
			mLocalRotation.z += 360.0f;
		}
	}


	/// <summary>
	/// 更新流程
	///
	/// 如果自己发生变化
	/// 先更新自己的数据
	/// 然后通知chilren强制更新数据
	///
	/// 如果自己没什么变化
	/// 就去看看children有没有变化
	/// </summary>
	void Transform::update()
	{
		if (mIsDirty)
		{
			mIsDirty = false;
			mDelegateUpdate(this);
			this->forceUpdateChildren();
		}
		else
		{
			this->updateChildren();
		}
	}

	void Transform::forceUpdate()
	{
		mIsDirty = false;
		mDelegateUpdate(this);
		this->forceUpdateChildren();
	}

	void Transform::forceUpdateChildren()
	{
		if (mChildren)
		{
			auto it = mChildren->begin();
			auto end = mChildren->end();
			while (it != end)
			{
				auto& child = (*it);
				if (child.lock())
				{
					child->forceUpdate();
					it++;
				}
				else
				{
					it = mChildren->erase(it);
					end = mChildren->end();
				}
			}
		}
	}

	void Transform::updateChildren()
	{
		if (mChildren)
		{
			auto it = mChildren->begin();
			auto end = mChildren->end();
			while (it != end)
			{
				auto& child = (*it);
				if (child.lock())
				{
					child->update();
					it++;
				}
				else
				{
					it = mChildren->erase(it);
					end = mChildren->end();
				}
			}
		}
	}

	void Transform::updateMatrix(Transform* self)
	{
		//mLocalRotationQ = quaternion(glm::radians(float3(mLocalRotation)));

		GLMHelper::buildQuaternion(mLocalRotationQ, mLocalRotation);

		if (mParent != nullptr)
		{
			mWorldPosition = mParent->mWorldScale * mLocalPosition;
			mWorldPosition = quaternion(glm::radians(mParent->mWorldRotation)) * mWorldPosition;
			mWorldPosition += mParent->mWorldPosition;

			mWorldRotationQ = mLocalRotationQ * quaternion(glm::radians(mParent->mWorldRotation));
			mWorldRotation = glm::degrees(glm::eulerAngles(mWorldRotationQ));

			mWorldScale = mParent->mWorldScale * mLocalScale;
		}
		else
		{
			mWorldPosition = mLocalPosition;
			mWorldRotation = mLocalRotation;
			mWorldScale = mLocalScale;
			mWorldRotationQ = mLocalRotationQ;
		}

		GLMHelper::buildMatrix(mModelMatrix, mWorldPosition, mWorldRotationQ, mWorldScale);
		//mModelMatrix = glm::translate(WORLD_MATRIX, mWorldPosition);
		//mModelMatrix = mModelMatrix * glm::mat4_cast(mWorldRotationQ);
		//mModelMatrix = glm::scale(mModelMatrix, mWorldScale);
	}

	void Transform::manualUpdateMatrix()
	{
		this->updateMatrix(this);
	}

	void Transform::addChild(Transform* transform)
	{
		if (mChildren == nullptr)
		{
			mChildren = new std::list<TinyWeakRef<Transform>>();
		}


		mChildren->emplace_back(transform);
	}

	bool Transform::removeChild(Transform* transform)
	{
		mChildren->erase(std::find_if(mChildren->begin(), mChildren->end(),
			[&transform](TinyWeakRef<Transform> inner)
			{
				return inner.get() == transform;
			}));

		return true;
	}

	float4x4 Transform::getWorldToLocalMatrix()
	{
		return glm::inverse(mModelMatrix);
	}

	void Transform::calculatePureLocalToWorldRotationMatrix(float3x3& outMatrix)
	{
		float3 temp_vec3;
		for (int i = 0; i < 3; i++)
		{
			temp_vec3 = mModelMatrix[i];
			outMatrix[i] = temp_vec3 / glm::length(temp_vec3);
		}
	}

	void Transform::transformPoint(const float3& local, float3& world) const
	{
		world = mWorldScale * local;
		world = mWorldRotationQ * world;
		world += mWorldPosition;
	}

	void Transform::transformVector(const float3& local, float3& world) const
	{
		//world = mModelMatrix * float4(local, 0.0f);
		world = mWorldRotationQ * local;
		world = world * mWorldScale;
	}

	void Transform::transformRotation(const float3& local, float3& world) const
	{
		world = glm::eulerAngles(quaternion(glm::radians(local)) * mWorldRotationQ);
		world = glm::degrees(world);
	}

	void Transform::transformDirection(const float3& local, float3& world) const
	{
		//float3x3 temp_mat;
		//this->calculatePureLocalToWorldRotationMatrix(temp_mat);
		//world = temp_mat * local;
		world = mWorldRotationQ * local;
	}

	void Transform::transformScale(const float3& local, float3& world) const
	{
		world = mWorldScale * local;
	}

	void Transform::inverseTransformPoint(const float3& world, float3& local) const
	{
		//local = glm::inverse(mModelMatrix) * float4(world, 1.0f);

		local = world - mWorldPosition;
		local = glm::inverse(mWorldRotationQ) * local;
		local = local / mWorldScale;
	}

	void Transform::inverseTransformVector(const float3& world, float3& local) const
	{
		//local = glm::inverse(mModelMatrix) * float4(world, 0.0f);

		local = glm::inverse(mWorldRotationQ) * local;
		local = local / mWorldScale;
	}

	void Transform::inverseTransformRotation(const float3& world, float3& local) const
	{
		local = glm::eulerAngles(quaternion(glm::radians(world)) * glm::inverse(mWorldRotationQ));
		local = glm::degrees(local);
	}

	void Transform::inverseTransformDirection(const float3& world, float3& local) const
	{
		//float3x3 temp_mat;
		//this->calculatePureLocalToWorldRotationMatrix(temp_mat);
		//local = glm::inverse(temp_mat) * local;

		local = glm::inverse(mWorldRotationQ) * world;
	}

	void Transform::inverseTransformScale(const float3& world, float3& local) const
	{
		local = world / mWorldScale;
	}

	float3 Transform::getWorldRotation() const
	{
		//float3x3 temp_mat;
		//this->calculatePureLocalToWorldRotationMatrix(temp_mat);
		//return glm::degrees(glm::eulerAngles(glm::quat_cast(temp_mat)));

		return mWorldRotation;
	}

	float3 Transform::getWorldScale() const
	{
		return mWorldScale;
		//return { glm::length(this->getRight()), glm::length(this->getUp()), glm::length(this->getBackward()) };
	}

	void Transform::decomposeTransform()
	{
		//float4x4 mat(mModelMatrix);
		if (mParent != nullptr)
		{
			//mat = glm::inverse(mParent->getModelMatrix()) * mModelMatrix;

			auto iq = glm::inverse(mParent->mWorldRotationQ);
			
			mLocalPosition = mWorldPosition - mParent->mWorldPosition;
			mLocalPosition = iq * mLocalPosition;
			mLocalPosition = mLocalPosition / mParent->mWorldScale;
			
			mLocalRotationQ = mWorldRotationQ * iq;
			mLocalRotation = glm::degrees(glm::eulerAngles(mLocalRotationQ));

			mLocalScale = mWorldScale / mParent->mWorldScale;

			//mParent->inverseTransformPoint(mWorldPosition, mLocalPosition);
			//mParent->inverseTransformRotation(mWorldRotation, mLocalRotation);
		}
		else
		{
			mLocalPosition = mWorldPosition;
			mLocalRotation = mWorldRotation;
			mLocalRotationQ = mWorldRotationQ;
			mLocalScale = mWorldScale;
		}
		//else
		//{
		//	mLocalPosition = mWorldPosition;
		//}

		//GLMHelper::decompose(mat, mLocalPosition, mLocalRotation, mLocalScale);
		//
		//if (glm::determinant(mat) < 0.0f)
		//{
		//	mLocalScale.x *= -1.0f;
		//}
	}

	void Transform::setWorldPosition(const float3& world)
	{
		mIsDirty = true;
		mWorldPosition = world;
		if (mParent)
		{
			mParent->inverseTransformPoint(world, mLocalPosition);
		}
		else
		{
			mLocalPosition = world;
		}
	}

	void Transform::setWorldRotation(const float3& worldRotation)
	{
		mIsDirty = true;
		mWorldRotation = worldRotation;
		if (mParent)
		{
			mParent->inverseTransformRotation(worldRotation, mLocalRotation);
		}
		else
		{
			mLocalRotation = worldRotation;
		}
	}

}
