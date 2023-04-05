#include "Transform.h"
#include "../Manager/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Component/GameObject.h"
#include "Utility/Utility.h"

namespace tezcat::Tiny::Core
{
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
		, mDelegateUpdate(std::bind(&Transform::updateMatrix, this))
	{
		if (mParent != nullptr)
		{
			mParent->addChild(this);
		}
	}

	Transform::~Transform()
	{
		mParent = nullptr;
	}

	void Transform::onStart()
	{
		if (mParent == nullptr)
		{
			SceneMgr::getInstance()->getCurrentScene()->addTransform(this);
		}
	}

	void Transform::onEnable()
	{
		this->getGameObject()->mTransform = this;
	}

	void Transform::onDisable()
	{

	}

	void Transform::onUpdate()
	{
		mDelegateUpdate();

		if (!mChildren.empty())
		{
			auto it = mChildren.begin();
			while (it != mChildren.end())
			{
				auto go = (*it)->getGameObject();
				if (go->needDelete())
				{
					it = mChildren.erase(it);
					go->close();
					delete go;
				}
				else
				{
					(*it)->update();
					it++;
				}
			}
		}
	}

	void Transform::updateMatrix()
	{
		if (mIsDirty)
		{
			mIsDirty = false;

			if (mParent == nullptr)
			{
				mModelMatrix = WORLD_MATRIX;
			}
			else
			{
				mModelMatrix = mParent->getModelMatrix();
			}

			mModelMatrix = glm::translate(mModelMatrix, mLocalPosition);
			// 			mModelMatrix = glm::rotate(mModelMatrix, m_Rotation.x, XAxis);
			// 			mModelMatrix = glm::rotate(mModelMatrix, m_Rotation.y, YAxis);
			// 			mModelMatrix = glm::rotate(mModelMatrix, m_Rotation.z, ZAxis);

			mModelMatrix *= glm::toMat4(glm::quat(glm::radians(mLocalRotation)));
			mModelMatrix = glm::scale(mModelMatrix, mLocalScale);
		}
	}

	void Transform::addChild(Transform* transform)
	{
		//		transform->mIndex = static_cast<uint32_t>(mChildren.size());
		mChildren.push_back(transform);
	}

	bool Transform::removeChild(Transform* transform)
	{
		mChildren.erase(std::find(mChildren.begin(), mChildren.end(), transform));

		// 		auto index = transform->mIndex;
		// 		if (mChildren[index] != transform)
		// 		{
		// 			return false;
		// 		}
		// 
		// 		mChildren.erase(mChildren.begin() + index);
		// 
		// 		for (int i = index; i < mChildren.size(); i++)
		// 		{
		// 			mChildren[i]->mIndex = i;
		// 		}

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

	void Transform::setParent(Transform* parent)
	{
		if (parent == mParent)
		{
			return;
		}

		//如果父节点不为空
		if (mParent != nullptr)
		{
			//从父节点中删除自己
			mParent->removeChild(this);
		}

		mParent = parent;
		//如果传入的父节点为空,说明需要添加到Scene中
		if (mParent == nullptr)
		{
			SceneMgr::getInstance()->getCurrentScene()->addTransform(this);
		}
		else
		{
			mParent->addChild(this);
		}
	}
}
