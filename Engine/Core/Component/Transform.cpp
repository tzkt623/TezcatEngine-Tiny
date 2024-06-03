#include "Transform.h"
#include "../Scene/Scene.h"
#include "../Manager/SceneManager.h"
#include "../Component/GameObject.h"
#include "../Tool/Tool.h"

//#define QUAT_YZX;

namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(Transform, ComponentT<Transform>)

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
		, mModelMatrix(1.0f)
		, mParent(parent)
		, mIndex(0)
		, mDelegateUpdate(std::bind(&Transform::updateMatrix, this, std::placeholders::_1))
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
			//得到在世界空间中的数据
			float3 world_position = this->getWorldPosition();
			float3 world_rotation = this->getWorldRotation();
			float3 world_scale = this->getWorldScale();

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
			//如果变到根节点上,直接使用世界坐标系
			if (mParent == nullptr)
			{
				mLocalPosition = world_position;
				mLocalRotation = world_rotation;
				mLocalScale = world_scale;

				mIndex = 0;
				SceneManager::getCurrentScene()->addTransform(this);
			}
			//如果没有变到根节点上,使用当前父节点的矩阵得到本地坐标
			else
			{
				mIndex = mParent->mIndex + 1;
				mParent->addChild(this);

				mParent->inverseTransform(world_position, world_rotation, world_scale, mLocalPosition, mLocalRotation, mLocalScale);

				// 				mParent->inverseTransformPoint(world_position, mLocalPosition);
				// 				mParent->inverseTransformVector(world_rotation, mLocalRotation);
				// 				mParent->inverseTransformVector(world_scale, mLocalScale);
			}
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
			while (it != mChildren->end())
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
				}
			}
		}
	}

	void Transform::updateChildren()
	{
		if (mChildren)
		{
			auto it = mChildren->begin();
			while (it != mChildren->end())
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
				}
			}
		}
	}

	void Transform::updateMatrix(Transform* self)
	{
#ifdef QUAT_YZX
		auto qx = glm::angleAxis(glm::radians(mLocalRotation.x), XAxis);
		auto qy = glm::angleAxis(glm::radians(mLocalRotation.y), YAxis);
		auto qz = glm::angleAxis(glm::radians(mLocalRotation.z), ZAxis);
		mModelMatrix = glm::translate(WORLD_MATRIX, mLocalPosition);
		//Unity Rotation Order
		mModelMatrix = mModelMatrix * glm::mat4_cast(qy * qx * qz);
		mModelMatrix = glm::scale(mModelMatrix, mLocalScale);
#else
		mModelMatrix = glm::translate(WORLD_MATRIX, mLocalPosition);
		mModelMatrix = mModelMatrix * glm::mat4_cast(glm::quat(glm::radians(mLocalRotation)));
		mModelMatrix = glm::scale(mModelMatrix, mLocalScale);
#endif // QUAT_YZX

		if (mParent != nullptr)
		{
			mModelMatrix = mParent->getModelMatrix() * mModelMatrix;
		}
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

	void Transform::transformPoint(const float3& local, float3& world)
	{
		//world = glm::quat(glm::radians(mLocalRotation)) * (local * mLocalScale) + mLocalPosition;

		world = mModelMatrix * float4(local, 1.0f);
	}

	void Transform::transformVector(const float3& local, float3& world)
	{
		world = mModelMatrix * float4(local, 0.0f);
	}

	void Transform::transformRotation(const float3& local, float3& world)
	{
		float3x3 temp_mat;
		this->calculatePureLocalToWorldRotationMatrix(temp_mat);
		glm::quat quat_parent_world_rotation(temp_mat);
		glm::quat quat_child_world_rotation(glm::radians(world));

		world = glm::degrees(glm::eulerAngles(quat_parent_world_rotation * quat_child_world_rotation));
	}

	void Transform::transformDirection(const float3& local, float3& world)
	{
		float3x3 temp_mat;
		this->calculatePureLocalToWorldRotationMatrix(temp_mat);
		world = temp_mat * local;	}

	void Transform::inverseTransformPoint(const float3& world, float3& local)
	{
		local = glm::inverse(mModelMatrix) * float4(world, 1.0f);
	}

	void Transform::inverseTransformVector(const float3& world, float3& local)
	{
		local = glm::inverse(mModelMatrix) * float4(world, 0.0f);
	}

	void Transform::inverseTransformRotation(const float3& world, float3& local)
	{
		float3x3 temp_mat;
		this->calculatePureLocalToWorldRotationMatrix(temp_mat);
		glm::quat quat_parent_world_rotation(temp_mat);
		glm::quat quat_child_world_rotation(glm::radians(world));

		local = glm::degrees(glm::eulerAngles(glm::inverse(quat_parent_world_rotation) * quat_child_world_rotation));
	}

	void Transform::inverseTransformDirection(const float3& world, float3& local)
	{
		float3x3 temp_mat;
		this->calculatePureLocalToWorldRotationMatrix(temp_mat);
		local = glm::inverse(temp_mat) * local;
	}

	float3 Transform::getWorldRotation()
	{
		float3x3 temp_mat;
		this->calculatePureLocalToWorldRotationMatrix(temp_mat);
		return glm::degrees(glm::eulerAngles(glm::quat_cast(temp_mat)));
	}

	float3 Transform::getWorldScale() const
	{
		return { glm::length(this->getRight()), glm::length(this->getUp()), glm::length(this->getBackward()) };
	}


	void Transform::inverseTransform(const float3& worldPosition, const float3& worldRotation, const float3& worldScale, float3& localPosition, float3& localRotation, float3& localScale)
	{
		localScale = worldScale / this->getWorldScale();

#ifdef QUAT_YZX
		auto rotate = this->getWorldRotation();
		auto qx = glm::angleAxis(glm::radians(rotate.x), XAxis);
		auto qy = glm::angleAxis(glm::radians(rotate.y), YAxis);
		auto qz = glm::angleAxis(glm::radians(rotate.z), ZAxis);

		auto rq_ = glm::normalize(glm::inverse(qy * qx * qz));

		auto qwx = glm::angleAxis(glm::radians(worldRotation.x), XAxis);
		auto qwy = glm::angleAxis(glm::radians(worldRotation.y), YAxis);
		auto qwz = glm::angleAxis(glm::radians(worldRotation.z), ZAxis);

		//!!!!this glm::eulerAngles order is XYZ, so the angles of localRotation is error;
		localRotation = glm::degrees(glm::eulerAngles((qwy * qwx * qwz) * rq_));
#else
		float3x3 temp_mat;
		this->calculatePureLocalToWorldRotationMatrix(temp_mat);
		glm::quat quat_parent_world_rotation(temp_mat);
		glm::quat quat_child_world_rotation(glm::radians(worldRotation));

		localRotation = glm::degrees(glm::eulerAngles(glm::inverse(quat_parent_world_rotation) * quat_child_world_rotation));
#endif // QUAT_YZX

		/*
		* equal to current mothed
		*
		* auto mat_parent = glm::inverse(mModelMatrix);
		* localPosition = mat_parent * float4(worldPosition, 1.0f);
		*/
		auto mat_parent = glm::inverse(mModelMatrix);
		localPosition = mat_parent * float4(worldPosition, 1.0f);

		//localPosition = glm::inverse(quat_parent_world_rotation) * (worldPosition - this->getWorldPosition());
		//localPosition *= localScale;

		//world = glm::quat(glm::radians(mLocalRotation)) * (local * mLocalScale) + mLocalPosition;
	}



	//-------------------------------------------------
	//
	//	TransformList
	//
	TransformList::TransformList()
	{

	}

	TransformList::~TransformList()
	{

	}

	void TransformList::pushFront(Transform* transform)
	{

	}

	void TransformList::pushBack(Transform* transform)
	{

	}
}
