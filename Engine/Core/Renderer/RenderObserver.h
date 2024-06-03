#pragma once

#include "../Base/TinyObject.h"
#include "../Head/CppHead.h"
#include "../Head/GLMHead.h"
#include "../Head/RenderConfig.h"

namespace tezcat::Tiny
{
	class BaseGraphics;
	class Shader;
	class Transform;
	class FrameBuffer;
	class ObserverPipelinePass;
	class GameObject;

	enum class ViewType : uint8
	{
		Screen = 0,
		Ortho,
		Perspective
	};

	/*
	* RenderObserver
	*
	* @brief 渲染观察者
	* @brief 观察者可以用它的视角来观察整个场景,并提供观察视角的数据
	* @brief 观察者可以指定它看到的图元被渲染到哪个帧缓存上
	* @brief 每个渲染观察者都拥有一套完整[Shader-Pass]缓存系统
	*/
	class TINY_API BaseRenderObserver : public TinyObject
	{
		TINY_ABSTRACT_OBJECT_H(BaseRenderObserver, TinyObject)
	protected:
		BaseRenderObserver();

	public:
		virtual ~BaseRenderObserver();
		virtual void submitViewMatrix(BaseGraphics* graphics, Shader* shader) = 0;

	public:
		virtual bool culling(GameObject* gameObject) { return true; }

		bool isDataMode() const { return mCullLayerList.empty(); }

		bool cullLayerMask(uint32 index)
		{
			auto mask = 1 << index;
			return (mCullMask & mask) == mask;
		}

		void setCullLayer(uint32 index)
		{
			mCullMask = 1 << index;
			mCullLayerList.clear();
			mCullLayerList.push_back(index);
		}

		void addCullLayer(uint32 index)
		{
			mCullMask |= (1 << index);
			mCullLayerList.push_back(index);
		}

		void removeCullLayer(uint32 index)
		{
			mCullMask &= ~(1 << index);
			mCullLayerList.erase(std::find(mCullLayerList.begin(), mCullLayerList.end(), index));
		}

		const std::vector<uint32>& getCullLayerList() const { return mCullLayerList; }

		void addToPipeline();
		void removeFromPipeline();
		bool isNeedRemove() const { return mNeedRemove; }
		void onExitPipeline();

	public:
		void setViewRect(int32 x, int32 y, int32 width, int32 height);
		ViewportInfo& getViewportInfo() { return mViewInfo; }

		float4x4& getProjectionMatrix() { return mProjectionMatrix; }

		virtual float4x4& getViewMatrix() = 0;
		virtual float4x4& getViewMatrix(int32 index) const = 0;
		virtual float4x4* getViewMatrixAry() const = 0;
		virtual int32 getViewMatrixArySize() const = 0;

		void setOrtho(float near, float far);
		void setPerspective(float fov, float near, float far);

		void setClearOption(ClearOptionID option) { mClearOption = (ClearOption)option; }
		const ClearOption& getClearOption() const { return mClearOption; }

	public:
		int32 getOrderID() const { return mOrder; }
		//Range[-127,128]
		void setOrderID(int8 val)
		{
			mOrder = val;
		}

		float getFOV() const { return mFOV; }
		void setFOV(float fov)
		{
			mFOV = fov;
			mDirty = true;
		}

		float getAspect() const { return (float)mViewInfo.Width / (float)mViewInfo.Height; }

		void setNear(float near)
		{
			mNearFace = near;
			mDirty = true;
		}
		float getNear() const { return mNearFace; }
		void setFar(float far)
		{
			mFarFace = far;
			mDirty = true;
		}
		float getFar() const { return mFarFace; }

		ViewType getViewType() const { return mViewType; }

	public:
		void setTransform(Transform* transform);
		Transform* getTransform() { return mTransform; }

		ObserverPipelinePass* createOrGetPass(Shader* shader);

		FrameBuffer* getFrameBuffer() { return mFrameBuffer; }
		void setFrameBuffer(FrameBuffer* frameBuffer);

		void setEnable(bool val);
		bool getEnable() const { return mEnable; }

		uint32 getUID() const { return mUID; }

	public:
		void updateObserverMatrix();

	protected:
		virtual void onClose() override;

	protected:
		Transform* mTransform;

	protected:
		uint32 mUID;
		bool mNeedRemove;
		bool mEnable;
		bool mDirty;
		int32 mOrder;
		ViewType mViewType;
		float mNearFace;
		float mFarFace;
		float mFOV;

		float4x4 mProjectionMatrix;
		ViewportInfo mViewInfo;

	protected:
		uint32 mCullMask;
		std::vector<uint32> mCullLayerList;
		ClearOption mClearOption;

	protected:
		FrameBuffer* mFrameBuffer;

	protected:
		std::vector<ObserverPipelinePass*> mPassCache;
		std::vector<ObserverPipelinePass*> mPassArray;
	};


	class TINY_API RenderObserver : public BaseRenderObserver
	{
		TINY_OBJECT_H(RenderObserver, BaseRenderObserver)
	protected:
		RenderObserver();
	public:
		virtual ~RenderObserver();

	public:
		virtual void submitViewMatrix(BaseGraphics* graphics, Shader* shader) override;
		virtual float4x4& getViewMatrix() { return mViewMatrix; }

		virtual float4x4& getViewMatrix(int32 index) const final { TINY_THROW("Fatal Error!"); }
		virtual float4x4* getViewMatrixAry() const final { TINY_THROW("Fatal Error!"); }
		virtual int32 getViewMatrixArySize() const final { TINY_THROW("Fatal Error!"); }

	protected:
		float4x4 mViewMatrix;
	};

	class TINY_API RenderObserverMultView : public BaseRenderObserver
	{
		TINY_OBJECT_H(RenderObserverMultView, BaseRenderObserver)
	protected:
		RenderObserverMultView();
	public:
		virtual ~RenderObserverMultView();
		void setViewMartixArray(float4x4* array, int32 size);

		virtual void submitViewMatrix(BaseGraphics* graphics, Shader* shader) override;

	public:
		virtual float4x4& getViewMatrix() override final { TINY_THROW("Fatal Error!"); }

		virtual float4x4& getViewMatrix(int32 index) const override { return mViewMatrixArray[index]; }
		virtual float4x4* getViewMatrixAry() const override { return mViewMatrixArray; }
		virtual int32 getViewMatrixArySize() const override { return mArraySize; }

	protected:
		float4x4* mViewMatrixArray;
		int mArraySize;
	};

#define TINY_RENDER_OBSERVER_FUNCTION(memberName)\
public:\
	BaseRenderObserver* getRenderObserver() { return memberName; }\
	const std::vector<uint32>& getCullLayerList() const { return memberName->getCullLayerList(); }\
	float getFOV() const { return memberName->getFOV(); }\
	float getNear() const { return memberName->getNear(); }\
	float getFar() const { return memberName->getFar(); }\
	float getAspect() const { return memberName->getAspect(); }\
	int32 getOrderID() const { return memberName->getOrderID(); }\
	ViewType getViewType() const { return memberName->getViewType(); }\
public:\
	void setOrderID(int32 val) { memberName->setOrderID(val); }\
	void setOrtho(float near, float far) { return memberName->setOrtho(near, far); }\
	void setPerspective(float fov, float near, float far) { return memberName->setPerspective(fov, near, far); }\
	void setClearOption(ClearOptionID option) { memberName->setClearOption(option); }\
	const ClearOption& getClearOption() const { return memberName->getClearOption(); }\
	void setViewRect(int32 x, int32 y, int32 width, int32 height) { memberName->setViewRect(x, y, width, height); }\
	void setCullLayer(uint32 index) { memberName->setCullLayer(index); }\
	void addCullLayer(uint32 index) { memberName->addCullLayer(index); }\
	void removeCullLayer(uint32 index) { memberName->removeCullLayer(index); }\
	void setFrameBuffer(FrameBuffer* frameBuffer) { memberName->setFrameBuffer(frameBuffer); }
}
