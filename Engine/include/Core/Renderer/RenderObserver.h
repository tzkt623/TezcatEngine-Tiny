#pragma once
/*
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
#include "../Base/TinyObject.h"
#include "../Head/TinyCpp.h"
#include "../Head/GLMHead.h"
#include "../Renderer/RenderConfig.h"

namespace tezcat::Tiny
{
	class Shader;
	class Transform;
	class FrameBuffer;
	class UniformBuffer;
	class GameObject;
	class PipelineQueue;

	enum class ViewType : uint8_t
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
	*/
	class TINY_API BaseRenderObserver : public TinyObject
	{
		TINY_ABSTRACT_OBJECT_H(BaseRenderObserver, TinyObject)
	protected:
		BaseRenderObserver();
		void init() override;

	public:
		virtual ~BaseRenderObserver();
		virtual void submit(Shader* shader) = 0;

	public:
		virtual void preRender() {}

		void createUniformBuffer();

		UniformBuffer* getUniformBuffer() const { return mUniformBuffer; }

		virtual bool culling(GameObject* gameObject) { return true; }

		bool isDataMode() const { return mCullLayerList.empty(); }

		bool cullLayerMask(uint32_t index) const
		{
			uint32_t mask = 1 << index;
			return (mCullMask & mask) == mask;
		}

		void setCullLayer(uint32_t index);

		void addCullLayer(uint32_t index);

		void removeCullLayer(uint32_t index);

		const std::vector<uint32_t>& getCullLayerList() const { return mCullLayerList; }

	public:
		void setViewRect(int32_t x, int32_t y, int32_t width, int32_t height);
		void setViewRect(int32_t width, int32_t height);
		ViewportInfo& getViewRect() { return mViewInfo; }

		float4x4& getProjectionMatrix() { return mProjectionMatrix; }

		virtual float4x4& getViewMatrix() = 0;
		virtual float4x4& getViewMatrix(int32_t index) const = 0;
		virtual float4x4* getViewMatrixAry() const = 0;
		virtual int32_t getViewMatrixArySize() const = 0;

		void setOrtho(float near, float far);
		void setPerspective(float fov, float near, float far);

		void setClearOption(ClearOptionID option) { mClearOption = (ClearOption)option; }
		const ClearOption& getClearOption() const { return mClearOption; }

		TinyUID getUID() { return mUID; }

	public:
		int32_t getSortingID() const { return mSortingID; }
		/*
		* 值越大越先渲染
		*/
		void setSortingID(int32_t val)
		{
			mSortingID = val;
			if (evtSortingIDChanged)
			{
				evtSortingIDChanged(mSortingID);
			}
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

		virtual void lookAt(const float3& worldPosition) {}

		PipelineQueue* getPipelineQueue() { return mQueue; }

	public:
		void setTransform(Transform* transform);
		Transform* getTransform() { return mTransform; }

		FrameBuffer* getFrameBuffer() { return mFrameBuffer; }
		void setFrameBuffer(FrameBuffer* frameBuffer);

		void setEnable(bool val);
		bool getEnable() const { return mEnable; }

		void setClearColor(float4 color) { mClearColor = color; }
		const float4& getClearColor() const { return mClearColor; }

	public:
		bool updateObserverMatrix();

	public:
		std::function<void(int32_t)> evtSortingIDChanged;

	protected:
		virtual void onClose() override;

	protected:
		Transform* mTransform;

	protected:
		bool mNeedRemove;
		bool mEnable;
		bool mDirty;
		int32_t mSortingID;
		ViewType mViewType;
		float mNearFace;
		float mFarFace;
		float mFOV;
		ClearOption mClearOption;
		float4 mClearColor;
		ViewportInfo mViewInfo;

		float4x4 mProjectionMatrix;

	protected:
		uint32_t mCullMask;
		std::vector<uint32_t> mCullLayerList;

	protected:
		PipelineQueue* mQueue;

	protected:
		FrameBuffer* mFrameBuffer;
		UniformBuffer* mUniformBuffer;

	protected:
		TinyUID mUID;
	};

	class TINY_API ShadowObserver : public BaseRenderObserver
	{
		TINY_OBJECT_H(ShadowObserver, BaseRenderObserver)
	protected:
		ShadowObserver();
	public:
		virtual ~ShadowObserver();

	public:
		void preRender() override;
		void submit(Shader* shader) override;
		float4x4& getViewMatrix() override { return mViewMatrix; }

		float4x4& getViewMatrix(int32_t index) const override final { TINY_THROW("Fatal Error!"); }
		float4x4* getViewMatrixAry() const override final { TINY_THROW("Fatal Error!"); }
		int32_t getViewMatrixArySize() const override final { TINY_THROW("Fatal Error!"); }

	private:
		float4x4 mViewMatrix;
	};

	class TINY_API RenderObserver : public BaseRenderObserver
	{
		TINY_OBJECT_H(RenderObserver, BaseRenderObserver)
	protected:
		RenderObserver();
	public:
		virtual ~RenderObserver();

	public:
		void preRender() override;
		void submit(Shader* shader) override;
		float4x4& getViewMatrix() override { return mViewMatrix; }

		float4x4& getViewMatrix(int32_t index) const override final { TINY_THROW("Fatal Error!"); }
		float4x4* getViewMatrixAry() const override final { TINY_THROW("Fatal Error!"); }
		int32_t getViewMatrixArySize() const override final { TINY_THROW("Fatal Error!"); }

		void lookAt(const float3& worldPosition) override;

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

		void setViewMartixArray(float4x4* array, int32_t size);

		void submit(Shader* shader) override;
		void preRender() override;

	public:
		float4x4& getViewMatrix() override final { TINY_THROW("Fatal Error!"); }

		float4x4& getViewMatrix(int32_t index) const override { return mViewMatrixArray[index]; }
		float4x4* getViewMatrixAry() const override { return mViewMatrixArray; }
		int32_t getViewMatrixArySize() const override { return mArraySize; }

	protected:
		float4x4* mViewMatrixArray;
		int mArraySize;
	};

#define TINY_RENDER_OBSERVER_FUNCTION(memberName)\
public:\
	BaseRenderObserver* getRenderObserver() { return memberName; }\
	const std::vector<uint32_t>& getCullLayerList() const { return memberName->getCullLayerList(); }\
	float getFOV() const { return memberName->getFOV(); }\
	float getNear() const { return memberName->getNear(); }\
	float getFar() const { return memberName->getFar(); }\
	float getAspect() const { return memberName->getAspect(); }\
	ViewportInfo& getViewRect() const { return memberName->getViewRect(); }\
	int32_t getSortingID() const { return memberName->getSortingID(); }\
	ViewType getViewType() const { return memberName->getViewType(); }\
	void setClearColor(const float4& color) { memberName->setClearColor(color); }\
	const float4& getClearColor() const { return memberName->getClearColor(); }\
	FrameBuffer* getFrameBuffer() { return memberName->getFrameBuffer(); }\
	void setFrameBuffer(FrameBuffer* frameBuffer) { memberName->setFrameBuffer(frameBuffer); }\
public:\
	void setSortingID(int32_t val) { memberName->setSortingID(val); }\
	void setOrtho(float near, float far) { return memberName->setOrtho(near, far); }\
	void setPerspective(float fov, float near, float far) { return memberName->setPerspective(fov, near, far); }\
	void setClearOption(ClearOptionID option) { memberName->setClearOption(option); }\
	const ClearOption& getClearOption() const { return memberName->getClearOption(); }\
	void setViewRect(int32_t x, int32_t y, int32_t width, int32_t height) { memberName->setViewRect(x, y, width, height); }\
	void setCullLayer(uint32_t index) { memberName->setCullLayer(index); }\
	void addCullLayer(uint32_t index) { memberName->addCullLayer(index); }\
	void removeCullLayer(uint32_t index) { memberName->removeCullLayer(index); }\
	void lookAt(const float3& worldPosotion) { memberName->lookAt(worldPosotion); }
}
