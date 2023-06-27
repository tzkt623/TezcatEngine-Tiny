#include "Transform.h"
#include "../Scene/Scene.h"
#include "../Manager/SceneManager.h"
#include "../Component/GameObject.h"
#include "../Tool/Tool.h"


namespace tezcat::Tiny
{
	TINY_RTTI_CPP(Transform);

	static glm::mat4 WORLD_MATRIX(1.0f);
	static glm::vec3 XAxis(1.0f, 0.0f, 0.0f);
	static glm::vec3 YAxis(0.0f, 1.0f, 0.0f);
	static glm::vec3 ZAxis(0.0f, 0.0f, 1.0f);

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
		mGameObject->swapTransform();
		SceneMgr::getInstance()->getCurrentScene()->addNewTransform(this);
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

		bool inited = mGameObject->getScene() != nullptr;

		//如果父节点不为空
		if (mParent != nullptr)
		{
			//从父节点中删除自己
			mParent->removeChild(this);
		}
		else
		{
			if (inited)
			{
				mGameObject->getScene()->getTransformList().remove_if([this](TinyWeakRef<Transform>& ptr)
				{
					return ptr.get() == this;
				});
			}
		}

		if (parent == nullptr)
		{
			mParent = parent;
			mIndex = 0;

			if (inited)
			{
				SceneMgr::getInstance()->getCurrentScene()->addTransform(this);
			}
			else
			{
				SceneMgr::getInstance()->getCurrentScene()->addNewTransform(this);
			}
		}
		else
		{
			mParent = parent;
			mIndex = mParent->mIndex + 1;
			mParent->addChild(this);
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
				auto child = (*it);
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
				auto child = (*it);
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
		mModelMatrix = glm::translate(WORLD_MATRIX, mLocalPosition);

		// 		auto q1 = glm::rotate(glm::quat(), mLocalRotation.x, this->getRight());
		// 		auto q2 = glm::rotate(glm::quat(), mLocalRotation.y, this->getUp());
		// 		auto q3 = glm::rotate(glm::quat(), mLocalRotation.z, this->getForward());
		// 		mModelMatrix = mModelMatrix * glm::mat4_cast(q1 * q2 * q3);

		mModelMatrix = mModelMatrix * glm::mat4_cast(glm::quat(glm::radians(mLocalRotation)));
		mModelMatrix = glm::scale(mModelMatrix, mLocalScale);

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
			[transform](TinyWeakRef<Transform> inner)
			{
				return inner.get() == transform;
			}));

		return true;
	}

	glm::mat4 Transform::getWorldToLocalMatrix()
	{
		return glm::inverse(mModelMatrix);
	}

	void Transform::transformPoint(glm::vec3& localPosition)
	{
		localPosition = mModelMatrix * glm::vec4(localPosition, 1.0f);
	}

	void Transform::transformPoint(const glm::vec3& localPosition, glm::vec3& worldPosition)
	{
		worldPosition = mModelMatrix * glm::vec4(localPosition, 1.0f);
	}

	void Transform::transformVector(glm::vec3& localVector)
	{
		localVector = mModelMatrix * glm::vec4(localVector, 0.0f);
	}

	void Transform::transformVector(const glm::vec3& localVector, glm::vec3& worldVector)
	{
		worldVector = mModelMatrix * glm::vec4(localVector, 0.0f);
	}

	void Transform::inverseTransformPoint(glm::vec3& worldPosition)
	{
		worldPosition = glm::inverse(mModelMatrix) * glm::vec4(worldPosition, 1.0f);
	}

	void Transform::inverseTransformPoint(const glm::vec3& worldPosition, glm::vec3& localPosition)
	{
		localPosition = glm::inverse(mModelMatrix) * glm::vec4(worldPosition, 1.0f);
	}

	void Transform::inverseTransformVector(glm::vec3& worldVector)
	{
		worldVector = glm::inverse(mModelMatrix) * glm::vec4(worldVector, 0.0f);
	}

	void Transform::inverseTransformVector(const glm::vec3& worldVector, glm::vec3& localVector)
	{
		localVector = glm::inverse(mModelMatrix) * glm::vec4(worldVector, 0.0f);
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
