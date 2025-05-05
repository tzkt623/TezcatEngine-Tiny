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

#include "Core/Renderer/CameraObserver.h"
#include "Core/Renderer/VertexBuffer.h"
#include "Core/Renderer/BaseGraphics.h"
#include "Core/Shader/ShaderParam.h"

#include "Core/Component/Transform.h"


namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(CameraObserver, RenderObserver);
	CameraObserver::CameraObserver()
		: mCameraUID(-1)
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

	CameraObserver::~CameraObserver()
	{

	}

	void CameraObserver::preRender()
	{
		if (!mTransform)
		{
			return;
		}

		if (this->updateObserverMatrix())
		{
			mUniformBuffer->updateWithConfig<UniformBufferBinding::Camera::MatrixP>(glm::value_ptr(mProjectionMatrix));
		}

		mViewMatrix = glm::lookAt(mTransform->getWorldPosition()
			, mTransform->getWorldPosition() + mTransform->getForward()
			, mTransform->getUp());

		auto VP = mProjectionMatrix * mViewMatrix;

		mUniformBuffer->updateWithConfig<UniformBufferBinding::Camera::MatrixV>(glm::value_ptr(mViewMatrix));
		mUniformBuffer->updateWithConfig<UniformBufferBinding::Camera::MatrixVP>(glm::value_ptr(VP));
		mUniformBuffer->updateWithConfig<UniformBufferBinding::Camera::WorldPosition>(glm::value_ptr(mTransform->getWorldPosition()));
		mUniformBuffer->updateWithConfig<UniformBufferBinding::Camera::NearFar>(glm::value_ptr(float2(mNearFace, mFarFace)));
	}

	void CameraObserver::submit(Shader* shader)
	{
		if (mUniformBuffer)
		{
			Graphics::getInstance()->updateUniformBuffer(mUniformBuffer);
			Graphics::getInstance()->bind(mUniformBuffer);
		}
	}

}
