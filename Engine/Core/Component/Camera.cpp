#include "Camera.h"
#include "Transform.h"
#include "Component.h"

#include "../Engine.h"

#include "../Scene/Scene.h"

#include "../Manager/CameraManager.h"
#include "../Manager/PipelineManager.h"
#include "../Manager/SceneManager.h"

#include "../Component/MeshRenderer.h"
#include "../Component/GameObject.h"
#include "../Shader/ShaderPackage.h"
#include "../Shader/Shader.h"

#include "../Renderer/BaseGraphics.h"
#include "../Renderer/FrameBuffer.h"
#include "../Renderer/LayerMask.h"

#include "../Pipeline/Pipeline.h"
#include "../Pipeline/Forward.h"

#include "../Data/Material.h"


#include "Utility/Utility.h"


namespace tezcat::Tiny::Core
{
	Camera::Camera(Pipeline* pipeline, bool mainCamera)
		: IRenderObserver()
		, mUID(Utility::IDGenerator<Camera, unsigned int>::generate())
		, mIsMain(mainCamera)
		//, mViewMatrix(1.0f)
		, mDepth(0)
		, mFront(0.0f, 0.0f, -1.0f)
		, mUp(0.0f, 1.0f, 0.0f)
		, mRight(1.0f, 0.0f, 0.0f)
		, mWorldUp(0.0f, 1.0f, 0.0f)
		, mYaw(-90.0f)
		, mPitch(0.0f)
		, mRoll(0.0f)
		, mClearMask(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth))
		, mPipeline(pipeline)
	{

	}

	Camera::Camera(Pipeline* pipeling)
		: Camera(pipeling, true)
	{

	}

	Camera::Camera(bool mainCamera)
		: Camera(PipelineMgr::getInstance()->get("Forward"), mainCamera)
	{
	}

	Camera::Camera()
		: Camera(PipelineMgr::getInstance()->get("Forward"), true)
	{
	}

	Camera::~Camera()
	{

	}

	void Camera::onStart()
	{
		this->getTransform()->setDelegateUpdate(std::bind(&Camera::updateTransform, this, std::placeholders::_1));
	}

	void Camera::onEnable()
	{
		CameraMgr::getInstance()->addCamera(this);
	}

	void Camera::onDisable()
	{

	}

	//注意处理这个函数的调用位置
	void Camera::updateTransform(Transform* transform)
	{
		if (mPMatDirty)
		{
			mPMatDirty = false;
			switch (mType)
			{
			case Type::Ortho:
				mProjectionMatrix = glm::ortho(
					(float)mViewInfo.OX, (float)mViewInfo.Width,
					(float)mViewInfo.OY, (float)mViewInfo.Height,
					mNearFace, mFarFace);
				break;
			case Type::Perspective:
				mProjectionMatrix = glm::perspective(
					glm::radians(mFOV),
					(float)mViewInfo.Width / (float)mViewInfo.Height,
					mNearFace, mFarFace);
				break;
			default:
				break;
			}
		}

		auto& position = transform->getPosition();
		auto& viewMat4 = transform->getModelMatrix();
		viewMat4 = glm::lookAt(position, position + mFront, mUp);

		if (transform->getParent() != nullptr)
		{
			viewMat4 = transform->getParent()->getModelMatrix() * viewMat4;
		}
	}

	//注意处理这个函数的调用位置
	void Camera::onUpdate()
	{
		if (mPMatDirty)
		{
			mPMatDirty = false;
			switch (mType)
			{
			case Type::Ortho:
				mProjectionMatrix = glm::ortho(
					(float)mViewInfo.OX, (float)mViewInfo.Width,
					(float)mViewInfo.OY, (float)mViewInfo.Height,
					mNearFace, mFarFace);
				break;
			case Type::Perspective:
				mProjectionMatrix = glm::perspective(
					glm::radians(mFOV),
					(float)mViewInfo.Width / (float)mViewInfo.Height,
					mNearFace, mFarFace);
				break;
			default:
				break;
			}
		}

		auto& position = this->getTransform()->getPosition();
		// 		mViewMatrix = glm::lookAt(position, position + mFront, mUp);
		// 
		// 		if (this->getTransform()->getParent() != nullptr)
		// 		{
		// 			mViewMatrix = this->getTransform()->getParent()->getModelMatrix() * mViewMatrix;
		// 		}
		//		this->getTransform()->setModelMatrix(mViewMatrix);
	}

	void Camera::setMain()
	{
		mIsMain = true;
		CameraMgr::getInstance()->setMainCamera(this);
	}

	void Camera::render(BaseGraphics* graphics)
	{
		mPipeline->render(graphics, this);
	}

	void Camera::submit(Shader* shader)
	{

	}

	void Camera::submitViewMatrix(Shader* shader)
	{
		auto& view_model = this->getTransform()->getModelMatrix();
		shader->setProjectionMatrix(mProjectionMatrix);
		shader->setViewMatrix(view_model);

		//glm::vec3 world_position(0.0f);
		//this->getTransform()->transformPoint(this->getTransform()->getPosition(), world_position);

		shader->setViewPosition(this->getTransform()->getWorldPosition());
		shader->setMat4(ShaderParam::MatrixSBV, glm::value_ptr(glm::mat4(glm::mat3(view_model))));
		shader->setFloat2(ShaderParam::ViewNearFar, glm::vec2(mNearFace, mFarFace));
	}

	void Camera::beginRender()
	{
		FrameBuffer::bind(mFrameBuffer);
	}

	void Camera::endRender()
	{
		FrameBuffer::unbind(mFrameBuffer);
	}

	void Camera::yawPitch(float yaw, float pitch, bool constrainPitch)
	{
		yaw *= 0.2f;
		pitch *= 0.2f;

		mYaw += yaw;
		mPitch += pitch;

		if (mYaw > 360)
		{
			mYaw -= 360;
		}
		else if (mYaw < -360)
		{
			mYaw += 360;
		}

		if (constrainPitch)
		{
			if (mPitch > 89.0f)
			{
				mPitch = 89.0f;
			}
			if (mPitch < -89.0f)
			{
				mPitch = -89.0f;
			}
		}

		this->getTransform()->setRotation(mPitch, mYaw, mRoll);

		this->updateVector();
	}

	void Camera::updateVector()
	{
		glm::vec3 front(0.0f);
		front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		front.y = sin(glm::radians(mPitch));
		front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		mFront = glm::normalize(front);

		mWorldUp = glm::normalize(mWorldUp * glm::angleAxis(glm::radians(mRoll), mFront));

		mRight = glm::normalize(glm::cross(mFront, mWorldUp));
		mUp = glm::normalize(glm::cross(mRight, mFront));
	}

	void Camera::roll(float speed)
	{
		mRoll += speed;
		if (mRoll)
		{
		}
		//auto crrrent_up = glm::normalize(mWorldUp * glm::angleAxis(glm::radians(mRoll), mFront));
		//mWorldUp = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f) * glm::angleAxis(glm::radians(mRoll), mFront));
		//mRight = glm::normalize(glm::cross(mFront, crrrent_up));
		//mUp = glm::normalize(glm::cross(mRight, mFront));
	}

	glm::mat4& Camera::getViewMatrix()
	{
		return mGameObject->getTransform()->getModelMatrix();
	}

}
