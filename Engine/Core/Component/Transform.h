#pragma once
#include "Component.h"
#include "../Head/CppHead.h"
#include "../Head/GLMHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class TINY_API Transform : public ComponentT<Transform>
	{
		friend class Scene;
	public:
		Transform();
		Transform(Transform* parent);
		~Transform();

		//用来解决相机矩阵的问题
		void setDelegateUpdate(const std::function<void()>& function)
		{
			mDelegateUpdate = function;
		}

		void setModelMatrix(const glm::mat4& mat4)
		{
			mModelMatrix = mat4;
		}

	public:
		glm::vec3& getPosition() { return mLocalPosition; }
		void setPosition(const glm::vec3& val)
		{
			mIsDirty = true;
			mLocalPosition = val;
		}

		void setPosition(float val[3])
		{
			mIsDirty = true;
			mLocalPosition.x = val[0];
			mLocalPosition.y = val[1];
			mLocalPosition.z = val[2];
		}

		glm::vec3& getRotation() { return mLocalRotation; }
		void setRotation(const glm::vec3& val)
		{
			mIsDirty = true;
			mLocalRotation = val;
		}

		void setRotation(float x, float y, float z)
		{
			mIsDirty = true;
			mLocalRotation.x = x;
			mLocalRotation.y = y;
			mLocalRotation.z = z;
		}

		glm::vec3& getScale() { return mLocalScale; }
		void setScale(const glm::vec3& val)
		{
			mIsDirty = true;
			mLocalScale = val;
		}

	public:
		void setWorldPosition(const glm::vec3& worldPosition)
		{
			mLocalPosition = worldPosition;
			this->inverseTransformPoint(mLocalPosition);
		}

		glm::vec3 getWorldPosition()
		{
			return mModelMatrix[3];
		}

		glm::vec3 getRight() const
		{
			return mModelMatrix[0];
		}

		glm::vec3 getUp() const
		{
			return mModelMatrix[1];
		}

		glm::vec3 getBackward() const
		{
			return mModelMatrix[2];
		}

		glm::vec3 getForward() const
		{
			return -mModelMatrix[2];
		}

		glm::vec3 getGlobalScale() const
		{
			return { glm::length(this->getRight()), glm::length(this->getUp()), glm::length(this->getBackward()) };
		}

	public:
		inline glm::mat4x4& getModelMatrix() { return mModelMatrix; }
		glm::mat4 getWorldToLocalMatrix();

		void transformPoint(glm::vec3& localPosition);
		void transformPoint(const glm::vec3& localPosition, glm::vec3& worldPosition);
		void transformVector(glm::vec3& localVector);
		void transformVector(const glm::vec3& localVector, glm::vec3& worldVector);

		void inverseTransformPoint(glm::vec3& worldPosition);
		void inverseTransformPoint(const glm::vec3& worldPosition, glm::vec3& localPosition);
		void inverseTransformVector(glm::vec3& worldVector);
		void inverseTransformVector(const glm::vec3& worldVector, glm::vec3& localVector);

	public:
		Transform* getParent() const { return mParent; }
		void setParent(Transform* parent);

		void addChild(Transform* val);
		bool removeChild(Transform* val);
		size_t getChildCount() { return mChildren.size(); }
		std::vector<Transform*>& getChildren() { return mChildren; }

	public:
		void translate(const glm::vec3& offset)
		{
			mIsDirty = true;
			mLocalPosition += offset;
		}

	protected:
		void onStart() override;
		void onEnable() override;
		void onDisable() override;
		void onUpdate() override;
		void updateMatrix();

	private:
		uint32_t mIndex;
		Transform* mParent;
		std::vector<Transform*> mChildren;

	private:
		bool mIsDirty;
		glm::vec3 mLocalPosition;
		glm::vec3 mLocalRotation;
		glm::vec3 mLocalScale;

		glm::mat4x4 mModelMatrix;

		std::function<void()> mDelegateUpdate;
	};
}
