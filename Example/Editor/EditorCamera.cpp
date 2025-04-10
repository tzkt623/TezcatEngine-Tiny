#include "EditorCamera.h"

namespace tezcat::Editor
{
	TINY_OBJECT_CPP(EditorCamera, RenderObserver);

	EditorCamera::EditorCamera()
	{

	}

	EditorCamera::~EditorCamera()
	{

	}

	void EditorCamera::init()
	{
		Base::init();
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

	void EditorCamera::preRender()
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

}

