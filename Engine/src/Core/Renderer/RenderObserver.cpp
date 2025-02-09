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
	{
		mCullLayerList.reserve(32);
	}

	BaseRenderObserver::~BaseRenderObserver()
	{

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

	void BaseRenderObserver::setViewRect(int32 x, int32 y, int32 width, int32 height)
	{
		mViewInfo.OX = x;
		mViewInfo.OY = y;
		mViewInfo.Width = width;
		mViewInfo.Height = height;
		mDirty = true;
	}

	void BaseRenderObserver::setViewRect(int32 width, int32 height)
	{
		mViewInfo.OX = 0;
		mViewInfo.OY = 0;
		mViewInfo.Width = width;
		mViewInfo.Height = height;
		mDirty = true;
	}

	void BaseRenderObserver::updateObserverMatrix()
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
		}
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
		if (mPassCache.size() <= shader->getUID())
		{
			mPassCache.resize(shader->getUID() + 1);
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
;	}

	void BaseRenderObserver::removeFromPipeline()
	{
		mNeedRemove = true;
	}

	void BaseRenderObserver::onExitPipeline()
	{
		mNeedRemove = false;
	}
#pragma endregion


#pragma region RenderObserver
	TINY_OBJECT_CPP(RenderObserver, BaseRenderObserver)

	RenderObserver::RenderObserver()
		: mViewMatrix(1.0f)
	{

	}

	RenderObserver::~RenderObserver()
	{

	}

	void RenderObserver::submitViewMatrix(Shader* shader)
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

		Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixP, mProjectionMatrix);
		Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixV, mViewMatrix);
		Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixVP, VP);
		//Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixMV, glm::value_ptr(glm::mat4(glm::mat3(mViewMatrix))));

		Graphics::getInstance()->setFloat3(shader, ShaderParam::CameraWorldPosition, mTransform->getWorldPosition());
		Graphics::getInstance()->setFloat2(shader, ShaderParam::CameraNearFar, float2(mNearFace, mFarFace));
	}

#pragma endregion



#pragma region RenderObserverMultView
	TINY_OBJECT_CPP(RenderObserverMultView, BaseRenderObserver)

		RenderObserverMultView::RenderObserverMultView()
		: mArraySize(-1)
		, mViewMatrixArray(nullptr)
	{

	}

	RenderObserverMultView::~RenderObserverMultView()
	{
		delete[] mViewMatrixArray;
	}

	void RenderObserverMultView::setViewMartixArray(float4x4* array, int32 size)
	{
		mViewMatrixArray = array;
		mArraySize = size;
	}

	void RenderObserverMultView::submitViewMatrix(Shader* shader)
	{
		this->updateObserverMatrix();
		Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixP, mProjectionMatrix);
		Graphics::getInstance()->setFloat2(shader, ShaderParam::CameraNearFar, float2(mNearFace, mFarFace));
	}

#pragma endregion
}
