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

#include "Core/Renderer/CameraObserver.h"
#include "Core/Renderer/VertexBuffer.h"
#include "Core/Renderer/BaseGraphics.h"
#include "Core/Shader/ShaderParam.h"

#include "Core/Component/Transform.h"


namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(CameraObserver, RenderObserver);
	CameraObserver::CameraObserver()
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

	void CameraObserver::prepareRender()
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
		//Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixP, mProjectionMatrix);
		//Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixV, mViewMatrix);
		//Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixVP, VP);
		//Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixMV, glm::value_ptr(glm::mat4(glm::mat3(mViewMatrix))));

		//Graphics::getInstance()->setFloat3(shader, ShaderParam::CameraWorldPosition, mTransform->getWorldPosition());
		//Graphics::getInstance()->setFloat2(shader, ShaderParam::CameraNearFar, float2(mNearFace, mFarFace));

// 		mUniformBuffer->update<float4x4>(1, glm::value_ptr(mViewMatrix));
// 		mUniformBuffer->update<float4x4>(2, glm::value_ptr(VP));
// 		mUniformBuffer->update<float3>  (3, glm::value_ptr(mTransform->getWorldPosition()));
// 		mUniformBuffer->update<float2>  (4, glm::value_ptr(float2(mNearFace, mFarFace)));


		if (mUniformBuffer)
		{
			Graphics::getInstance()->setUniformBuffer(mUniformBuffer);
			Graphics::getInstance()->bind(mUniformBuffer);
		}
	}

}
