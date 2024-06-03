#pragma once
#include "Component.h"
#include "../Head/CppHead.h"
#include "../Head/GLMHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny
{
	/*
	* M_world = M_parent * M_child
	* M_parent^-1 * M_world =  M_parent^-1 * M_parent * M_child
	* M_parent^-1 * M_world = M_child
	*
	* 如果存储的是世界坐标系下的数据
	* 要得到此Transform在本地坐标系下的数据
	* 只需要Inverse父节点的数据然后计算就行了
	*
	* 如果存储的本地坐标系下的数据
	* 要得到此Transform在世界坐标系的下的数据
	* 需要得到他所有的父类并且依次进行计算
	*
	* glm euler Angle order is XYZ!!!!
	*/
	class TINY_API Transform : public ComponentT<Transform>
	{
		friend class Scene;
		Transform();
		Transform(Transform* parent);

		TINY_OBJECT_H(Transform, ComponentT<Transform>)

	public:
		virtual ~Transform();

		//用来解决相机矩阵的问题
		void setDelegateUpdate(const std::function<void(Transform*)>& function)
		{
			mDelegateUpdate = function;
		}

		void setModelMatrix(const float4x4& mat4)
		{
			mModelMatrix = mat4;
			this->updateChildren();
		}

		const uint32_t& getIndex() const { return mIndex; }
		void updateChildren();

	public:
		float3& getPosition() { return mLocalPosition; }
		void setPosition(const float3& val)
		{
			mIsDirty = true;
			mLocalPosition = val;
		}

		void setPosition(const float& x, const float& y, const float& z)
		{
			mIsDirty = true;
			mLocalPosition.x = x;
			mLocalPosition.y = y;
			mLocalPosition.z = z;
		}

		float3& getRotation()
		{
			return mLocalRotation;
		}

		void setRotation(const float3& val)
		{
			mIsDirty = true;
			mLocalRotation = val;
			this->clampRotation();
		}

		void setRotation(const float& x, const float& y)
		{
			mIsDirty = true;
			mLocalRotation.x = x;
			mLocalRotation.y = y;
			this->clampRotation();
		}

		void setRotation(const float& x, const float& y, const float& z)
		{
			mIsDirty = true;
			mLocalRotation.x = x;
			mLocalRotation.y = y;
			mLocalRotation.z = z;
			this->clampRotation();
		}

		float3& getScale() { return mLocalScale; }

		void setScale(const float3& val)
		{
			mIsDirty = true;
			mLocalScale = val;
		}

		void setScale(float x, float y, float z)
		{
			mIsDirty = true;
			mLocalScale.x = x;
			mLocalScale.y = y;
			mLocalScale.z = z;
		}

	public:
		float4x4& getModelMatrix() { return mModelMatrix; }

		float4x4 getWorldToLocalMatrix();
		const float4x4& getLocalToWorldMatrix() const { return mModelMatrix; }

		void setWorldPosition(const float3& world)
		{
			mIsDirty = true;
			this->inverseTransformPoint(world, mLocalPosition);
		}

		void setWorldRotation(const float3& worldRotation)
		{
			mIsDirty = true;
			this->inverseTransformVector(worldRotation, mLocalRotation);
		}

		float3 getRight() const { return mModelMatrix[0]; }
		float3 getLeft() const { return -mModelMatrix[0]; }
		float3 getUp() const { return mModelMatrix[1]; }
		float3 getDown() const { return -mModelMatrix[1]; }
		float3 getBackward() const { return mModelMatrix[2]; }
		float3 getForward() const { return -mModelMatrix[2]; }

		float3 getWorldRotation();
		float3 getWorldPosition() const { return mModelMatrix[3]; }
		float3 getWorldScale() const;

	public:
		void transformPoint(const float3& local, float3& world);
		void transformVector(const float3& local, float3& world);
		void transformRotation(const float3& local, float3& world);
		//朝向永远以原点作为中心
		void transformDirection(const float3& local, float3& world);

		//转化一个点,受位移,缩放和旋转影响
		void inverseTransformPoint(const float3& world, float3& local);
		//转化一个向量,受缩放和旋转影响
		void inverseTransformVector(const float3& world, float3& local);
		void inverseTransformRotation(const float3& world, float3& local);

		//朝向永远以原点作为中心
		void inverseTransformDirection(const float3& world, float3& local);

	private:
		void inverseTransform(const float3& worldPosition, const float3& worldRotation, const float3& worldScale, float3& localPosition, float3& localRotation, float3& localScale);
		void calculatePureLocalToWorldRotationMatrix(float3x3& outMatrix);

	public:
		Transform* getParent() const { return mParent; }

		/*
		* @author HCL
		* @info 2023|7|5
		* @brief 始终保持WorldSpace的状态
		*/
		void setParent(Transform* parent);

		void addChild(Transform* tansform);
		bool removeChild(Transform* tansform);

		size_t getChildCount() { return mChildren->size(); }
		std::list<TinyWeakRef<Transform>>* getChildren() { return mChildren; }

		void manualUpdateMatrix();
		void markDirty() { mIsDirty = true; }

		void update();

	public:
		void translate(const float3& offset)
		{
			mIsDirty = true;
			mLocalPosition += offset;
		}

		void rotate(const float3& offset)
		{
			mIsDirty = true;
			mLocalRotation += offset;
			this->clampRotation();
		}

		void clampRotation();

	protected:
		void onStart() override;
		void onEnable() override;
		void onDisable() override;

		void updateMatrix(Transform* self);

	public:
		void forceUpdate();
		void forceUpdateChildren();

	private:
		uint32_t mIndex;
		Transform* mParent;
		std::list<TinyWeakRef<Transform>>* mChildren;

	private:
		bool mIsDirty;
		float3 mLocalPosition;
		float3 mLocalRotation;
		float3 mLocalScale;
		float4x4 mModelMatrix;

		std::function<void(Transform*)> mDelegateUpdate;

	public:
		static const float3 XAxis;
		static const float3 YAxis;
		static const float3 ZAxis;
	};


	//-------------------------------------------------
	//
	//	TransformList
	//
	class TransformList
	{
	public:
		TransformList();
		~TransformList();

	public:
		void pushFront(Transform* transform);
		void pushBack(Transform* transform);

	private:
		Transform* mFront;
		Transform* mBack;
	};


	class TINY_API Collider
	{

	};
}
