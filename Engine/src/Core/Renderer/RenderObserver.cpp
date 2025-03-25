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

#include "Core/Renderer/RenderObserver.h"

#include "Core/Renderer/BaseGraphics.h"
#include "Core/Renderer/FrameBuffer.h"
#include "Core/Renderer/Pipeline.h"
#include "Core/Renderer/VertexBuffer.h"

#include "Core/Shader/Shader.h"
#include "Core/Shader/ShaderParam.h"

#include "Core/Component/Transform.h"

#include "Core/Manager/CameraManager.h"

namespace tezcat::Tiny
{
#pragma region BaseRenderObserver
	TINY_OBJECT_CPP(BaseRenderObserver, TinyObject)

		BaseRenderObserver::BaseRenderObserver()
		: mCullMask(0)
		, mFrameBuffer(nullptr)
		, mNearFace(0.1f)
		, mFarFace(100.0f)
		, mFOV(60.0f)
		, mProjectionMatrix(1.0f)
		, mViewType(ViewType::Screen)
		, mOrder(0)
		, mDirty(true)
		, mClearOption(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth))
		, mTransform(nullptr)
		, mEnable(true)
		, mNeedRemove(false)
		, mUID(0)
		, mUniformBuffer(nullptr)
	{
		mCullLayerList.reserve(32);
	}

	BaseRenderObserver::~BaseRenderObserver()
	{
		if (mUniformBuffer)
		{
			mUniformBuffer->deleteObject();
			mUniformBuffer = nullptr;
		}
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

	ObserverPipelinePass* BaseRenderObserver::createOrGetPass(Shader* shader)
	{
		auto uid = shader->getUID();
		if (mPassCache.size() <= uid)
		{
			mPassCache.resize(static_cast<size_t>(uid + 1));
		}

		if (mPassCache[uid] == nullptr)
		{
			ObserverPipelinePass* pass = ObserverPipelinePass::create(this, shader);
			pass->saveObject();
			mPassCache[uid] = pass;
			mPassArray.push_back(pass);
			if (mEnable)
			{
				pass->addToPipeline();
			}
		}

		return mPassCache[uid];
	}

	void BaseRenderObserver::setTransform(Transform* transform)
	{
		mTransform = transform;
		mTransform->saveObject();
	}

	void BaseRenderObserver::onClose()
	{
		for (auto pass : mPassCache)
		{
			if (pass)
			{
				pass->deleteObject();
			}
		}
		mPassCache.clear();
		mPassArray.clear();

		if (mFrameBuffer)
		{
			mFrameBuffer->deleteObject();
		}

		if (mTransform)
		{
			mTransform->deleteObject();
		}
	}

	void BaseRenderObserver::setEnable(bool val)
	{
		if (mEnable == val)
		{
			return;
		}

		mEnable = val;

		if (mEnable)
		{
			for (auto pass : mPassArray)
			{
				pass->addToPipeline();
			}
		}
		else
		{
			for (auto pass : mPassArray)
			{
				pass->removeFromPipeline();
			}
		}
	}

	void BaseRenderObserver::addToPipeline()
	{
		CameraManager::addRenderObserver(this);
	}

	void BaseRenderObserver::removeFromPipeline()
	{
		mNeedRemove = true;
	}

	void BaseRenderObserver::onExitPipeline()
	{
		mNeedRemove = false;
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
		if (!mTransform)
		{
			return;
		}

		this->updateObserverMatrix();

		mViewMatrix = glm::lookAt(mTransform->getWorldPosition()
			, mTransform->getWorldPosition() + mTransform->getForward()
			, mTransform->getUp());

		auto VP = mProjectionMatrix * mViewMatrix;

		//Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixP, mProjectionMatrix);
		//Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixV, mViewMatrix);
		//Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixVP, VP);
		//Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixMV, glm::value_ptr(glm::mat4(glm::mat3(mViewMatrix))));

		Graphics::getInstance()->setFloat3(shader, ShaderParam::CameraWorldPosition, mTransform->getWorldPosition());
		Graphics::getInstance()->setFloat2(shader, ShaderParam::CameraNearFar, float2(mNearFace, mFarFace));



		//auto of_mat = sizeof(float4x4) / sizeof(float) * 3;
		//float3 *p1 = (float3*)((float*)mUniformBuffer->getData() + of_mat);
		//float2 *p2 = (float2*)((float*)mUniformBuffer->getData() + of_mat + sizeof(float3) / sizeof(float));
		//
		//auto c = (*p1).x;

		if (mUniformBuffer)
		{
			Graphics::getInstance()->setUniformBuffer(mUniformBuffer);
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

			auto VP = mProjectionMatrix * mViewMatrix;

			if (mUniformBuffer)
			{
				mUniformBuffer->update<float4x4>(0, glm::value_ptr(mProjectionMatrix));
				mUniformBuffer->update<float4x4>(1, glm::value_ptr(mViewMatrix));
				mUniformBuffer->update<float4x4>(2, glm::value_ptr(VP));
				mUniformBuffer->update<float3>(3, glm::value_ptr(mTransform->getWorldPosition()));
				mUniformBuffer->update<float2>(4, glm::value_ptr(float2(mNearFace, mFarFace)));
			}
		}
	}

	void RenderObserver::lookAt(Transform* transform)
	{
		//auto dir = transform->getWorldPosition() - this->getTransform()->getWorldPosition();
		//dir = glm::normalize(dir);
		//auto r = glm::dot(dir, mTransform->getForward());
		//mTransform->setWorldRotation()
		//
		//mViewMatrix = glm::lookAt(mTransform->getWorldPosition()
		//	, transform->getWorldPosition()
		//	, mTransform->getUp());
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
			Graphics::getInstance()->setUniformBuffer(mUniformBuffer);
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
				layout->pushLayoutWithConfig<UniformBufferBinding::Camera::MatrixVP>();		//VP
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
		Graphics::getInstance()->setUniformBuffer(mUniformBuffer);
		Graphics::getInstance()->bind(mUniformBuffer);
	}
}
