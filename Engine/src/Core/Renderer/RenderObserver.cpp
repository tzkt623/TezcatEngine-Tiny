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

#include "Core/Renderer/RenderObserver.h"

#include "Core/Renderer/BaseGraphics.h"
#include "Core/Renderer/FrameBuffer.h"
#include "Core/Renderer/VertexBuffer.h"
#include "Core/Renderer/PipelineWorker.h"

#include "Core/Shader/Shader.h"
#include "Core/Shader/ShaderParam.h"

#include "Core/Component/Transform.h"

#include "Core/Manager/CameraManager.h"
#include "Core/Manager/ObserverManager.h"

namespace tezcat::Tiny
{
#pragma region BaseRenderObserver
	TINY_OBJECT_CPP(BaseRenderObserver, TinyObject);

	BaseRenderObserver::BaseRenderObserver()
		: mCullMask(0)
		, mFrameBuffer(nullptr)
		, mNearFace(0.1f)
		, mFarFace(100.0f)
		, mFOV(70.0f)
		, mProjectionMatrix(1.0f)
		, mViewType(ViewType::Screen)
		, mSortingID(0)
		, mDirty(true)
		, mClearOption(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth))
		, mTransform(nullptr)
		, mEnable(true)
		, mNeedRemove(false)
		, mUniformBuffer(nullptr)
		, mClearColor(0.0f, 0.0f, 0.0f, 1.0f)
		, mQueue(PipelineQueue::create())
	{
		mCullLayerList.reserve(32);
		mUID = ObserverManager::giveID();
	}

	BaseRenderObserver::~BaseRenderObserver()
	{

	}


	void BaseRenderObserver::init()
	{
		mQueue->setObserver(this);
		mQueue->saveObject();
	}

	void BaseRenderObserver::setOrtho(float near, float far)
	{
		mViewType = ViewType::Ortho;
		mNearFace = near;
		mFarFace = far;
		mDirty = true;
	}

	void BaseRenderObserver::setPerspective(float fov, float near, float far)
	{
		mViewType = ViewType::Perspective;
		mFOV = fov;
		mNearFace = near;
		mFarFace = far;
		mDirty = true;
	}

	void BaseRenderObserver::setViewRect(int32_t x, int32_t y, int32_t width, int32_t height)
	{
		mViewInfo.OX = x;
		mViewInfo.OY = y;
		mViewInfo.Width = width;
		mViewInfo.Height = height;
		mDirty = true;
	}

	void BaseRenderObserver::setViewRect(int32_t width, int32_t height)
	{
		mViewInfo.OX = 0;
		mViewInfo.OY = 0;
		mViewInfo.Width = width;
		mViewInfo.Height = height;
		mDirty = true;
	}

	bool BaseRenderObserver::updateObserverMatrix()
	{
		if (mDirty)
		{
			mDirty = false;
			switch (mViewType)
			{
			case ViewType::Ortho:
			{
				auto hw = mViewInfo.Width / 2.0f;
				auto hh = mViewInfo.Height / 2.0f;
				mProjectionMatrix = glm::ortho(
					mViewInfo.OX - hw, mViewInfo.OX + hw,
					mViewInfo.OY - hh, mViewInfo.OY + hh,
					mNearFace, mFarFace);
				break;
			}
			case ViewType::Perspective:
			{
				mProjectionMatrix = glm::perspective(
					glm::radians(mFOV),
					(float)mViewInfo.Width / (float)mViewInfo.Height,
					mNearFace, mFarFace);
				break;
			}
			default:
				break;
			}

			return true;
		}

		return false;
	}

	void BaseRenderObserver::setFrameBuffer(FrameBuffer* frameBuffer)
	{
		if (mFrameBuffer != nullptr)
		{
			mFrameBuffer->deleteObject();
		}

		mFrameBuffer = frameBuffer;

		if (mFrameBuffer)
		{
			mFrameBuffer->saveObject();
		}
	}

	void BaseRenderObserver::setTransform(Transform* transform)
	{
		mTransform = transform;
		mTransform->saveObject();
	}

	void BaseRenderObserver::onClose()
	{
		if (mFrameBuffer)
		{
			mFrameBuffer->deleteObject();
		}

		if (mTransform)
		{
			mTransform->deleteObject();
		}

		if (mUniformBuffer)
		{
			mUniformBuffer->deleteObject();
			mUniformBuffer = nullptr;
		}

		ObserverManager::recycle(mUID);

		mQueue->removeFromPipeline();
		mQueue->deleteObject();
	}

	void BaseRenderObserver::setEnable(bool val)
	{
		if (mEnable == val)
		{
			return;
		}

		mEnable = val;
	}

	void BaseRenderObserver::createUniformBuffer()
	{
		if (mUniformBuffer != nullptr)
		{
			return;
		}

		mUniformBuffer = UniformBuffer::create();
		mUniformBuffer->saveObject();
	}

	void BaseRenderObserver::setCullLayer(uint32_t index)
	{
		mCullMask = 1 << index;
		mCullLayerList.clear();
		mCullLayerList.push_back(index);
	}

	void BaseRenderObserver::addCullLayer(uint32_t index)
	{
		mCullMask |= (1 << index);
		mCullLayerList.push_back(index);
	}

	void BaseRenderObserver::removeCullLayer(uint32_t index)
	{
		mCullMask &= ~(1 << index);
		mCullLayerList.erase(std::find(mCullLayerList.begin(), mCullLayerList.end(), index));
	}


#pragma endregion


#pragma region RenderObserver
	TINY_OBJECT_CPP(RenderObserver, BaseRenderObserver);

	RenderObserver::RenderObserver()
		: mViewMatrix(1.0f)
	{
	}

	RenderObserver::~RenderObserver()
	{
	}

	void RenderObserver::submit(Shader* shader)
	{
		if (mUniformBuffer)
		{
			Graphics::getInstance()->bind(mUniformBuffer);
			Graphics::getInstance()->updateUniformBuffer(mUniformBuffer);
		}
	}

	void RenderObserver::preRender()
	{
		this->updateObserverMatrix();

		if (mTransform)
		{
			mViewMatrix = glm::lookAt(mTransform->getWorldPosition()
				, mTransform->getWorldPosition() + mTransform->getForward()
				, mTransform->getUp());


			//if (mUniformBuffer)
			//{
			//	auto VP = mProjectionMatrix * mViewMatrix;
			//	mUniformBuffer->update<float4x4>(0, glm::value_ptr(mProjectionMatrix));
			//	mUniformBuffer->update<float4x4>(1, glm::value_ptr(mViewMatrix));
			//	mUniformBuffer->update<float4x4>(2, glm::value_ptr(VP));
			//	mUniformBuffer->update<float3>(3, glm::value_ptr(mTransform->getWorldPosition()));
			//	mUniformBuffer->update<float2>(4, glm::value_ptr(float2(mNearFace, mFarFace)));
			//}
		}
	}

	void RenderObserver::lookAt(const float3& worldPosition)
	{
		//auto dir = transform->getWorldPosition() - this->getTransform()->getWorldPosition();
		//dir = glm::normalize(dir);
		//auto r = glm::dot(dir, mTransform->getForward());
		//mTransform->setWorldRotation()
		//
		//mViewMatrix = glm::lookAt(mTransform->getWorldPosition()
		//	, transform->getWorldPosition()
		//	, mTransform->getUp());

		//得到当前要看的位置
		//mTransform->setWorldPosition(worldPosition - mTransform->getForward() * 10.0f);

		//mViewMatrix = glm::lookAt(worldPosition, worldPosition + this->getf)
	}

#pragma endregion



#pragma region RenderObserverMultView
	TINY_OBJECT_CPP(RenderObserverMultView, BaseRenderObserver);

	RenderObserverMultView::RenderObserverMultView()
		: mArraySize(-1)
		, mViewMatrixArray(nullptr)
	{

	}

	RenderObserverMultView::~RenderObserverMultView()
	{
		delete[] mViewMatrixArray;
	}

	void RenderObserverMultView::setViewMartixArray(float4x4* array, int32_t size)
	{
		mViewMatrixArray = array;
		mArraySize = size;
	}

	void RenderObserverMultView::submit(Shader* shader)
	{
		//Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixP, mProjectionMatrix);
		//Graphics::getInstance()->setFloat2(shader, ShaderParam::CameraNearFar, float2(mNearFace, mFarFace));
		if (mUniformBuffer)
		{
			Graphics::getInstance()->updateUniformBuffer(mUniformBuffer);
			Graphics::getInstance()->bind(mUniformBuffer);
		}
	}

	void RenderObserverMultView::preRender()
	{
		if (this->updateObserverMatrix())
		{
			if (mUniformBuffer)
			{
				mUniformBuffer->updateWithConfig<UniformBufferBinding::SkyBox::MatrixP>(glm::value_ptr(mProjectionMatrix));
			}
		}
	}

#pragma endregion
	TINY_OBJECT_CPP(ShadowObserver, BaseRenderObserver);
	ShadowObserver::ShadowObserver()
	{
		this->createUniformBuffer();
		mUniformBuffer->setLayout("CameraUBO"
			, [](UniformBufferLayout* layout)
			{
				layout->pushLayoutWithConfig<UniformBufferBinding::Camera::MatrixP>();			//P
				layout->pushLayoutWithConfig<UniformBufferBinding::Camera::MatrixV>();			//V
				layout->pushLayoutWithConfig<UniformBufferBinding::Camera::MatrixVP>();			//VP
				layout->pushLayoutWithConfig<UniformBufferBinding::Camera::WorldPosition>();	//Position
				layout->pushLayoutWithConfig<UniformBufferBinding::Camera::NearFar>();			//NearFar
			});
	}

	ShadowObserver::~ShadowObserver()
	{
	}

	void ShadowObserver::preRender()
	{
		if (!mTransform)
		{
			return;
		}

		this->updateObserverMatrix();

		mViewMatrix = glm::lookAt(mTransform->getWorldPosition()
			, mTransform->getWorldPosition() + mTransform->getForward()
			, mTransform->getUp());

		auto vp = mProjectionMatrix * mViewMatrix;
		//mUniformBuffer->updateWithConfig<UniformBufferBinding::Camera::MatrixP>(glm::value_ptr(mProjectionMatrix));
		mUniformBuffer->updateWithConfig<UniformBufferBinding::Camera::MatrixVP>(glm::value_ptr(vp));
	}

	void ShadowObserver::submit(Shader* shader)
	{
		Graphics::getInstance()->updateUniformBuffer(mUniformBuffer);
		Graphics::getInstance()->bind(mUniformBuffer);
	}
}
