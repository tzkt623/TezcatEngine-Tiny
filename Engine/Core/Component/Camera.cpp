#include "Camera.h"
#include "Transform.h"
#include "Component.h"

#include "../Engine.h"

#include "../Scene/Scene.h"

#include "../Manager/CameraManager.h"
#include "../Manager/PipelineManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/LightManager.h"

#include "../Component/MeshRenderer.h"
#include "../Component/GameObject.h"
#include "../Component/Light.h"


#include "../Shader/ShaderPackage.h"
#include "../Shader/Shader.h"

#include "../Renderer/BaseGraphics.h"
#include "../Renderer/FrameBuffer.h"
#include "../Renderer/LayerMask.h"
#include "../Renderer/ShadowRenderer.h"

#include "../Layer/RenderLayer.h"

#include "../Pipeline/Pipeline.h"
#include "../Pipeline/Forward.h"

#include "../Data/Material.h"


#include "../Tool/Tool.h"


namespace tezcat::Tiny
{
	TINY_RTTI_CPP(Camera)

	Camera::Camera(Pipeline* pipeline, bool mainCamera)
		: IRenderObserver()
		, mUID(IDGenerator<Camera, unsigned int>::generate())
		, mIsMain(mainCamera)
		, mViewMatrix(1.0f)
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
		//this->getTransform()->setDelegateUpdate(std::bind(&Camera::updateTransform, this, std::placeholders::_1));
	}

	void Camera::onEnable()
	{
		CameraMgr::getInstance()->addCamera(this);
	}

	void Camera::onDisable()
	{

	}

	// 相机先算出自己的viewMatrix
	// 然后再逆这个viewMatrix得到相机的modelMatrix
	// 然后把这个modelMatrix发送给transform更新
	// 注意处理这个函数的调用位置
	void Camera::updateTransform(Transform* transform)
	{
		this->updateObserverMatrix();
		mViewMatrix = glm::lookAt(
			transform->getWorldPosition(),
			transform->getWorldPosition() * transform->getForward(),
			transform->getUp());
		transform->setModelMatrix(glm::inverse(mViewMatrix));
		//transform->manualUpdateMatrix();
		transform->forceUpdateChildren();
	}

	void Camera::setMain()
	{
		mIsMain = true;
		CameraMgr::getInstance()->setMainCamera(this);
	}

	void Camera::render(BaseGraphics* graphics)
	{
		//阴影渲染
		//阴影不需要背景层的物体
		auto dir_light = LightMgr::getInstance()->getDirectionalLight();
		if (dir_light != nullptr)
		{
			auto& dir_light_culls = dir_light->getCullLayerList();

			//剔除到阴影通道
			for (auto index : dir_light_culls)
			{
				RenderLayer::getRenderLayer(index)->culling(dir_light, RenderPassType::Shadow);
			}

			ShadowRenderer::beginRender();
			ShadowRenderer::render(graphics, dir_light);
			ShadowRenderer::endRender();
		}


		mPipeline->render(graphics, this);
	}

	void Camera::submit(Shader* shader)
	{

	}

	void Camera::submitViewMatrix(Shader* shader)
	{
		this->updateObserverMatrix();
		auto transform = this->getTransform();

		mViewMatrix = glm::lookAt(
			transform->getPosition(),
			transform->getPosition() + transform->getForward(),
			transform->getUp());

 		if (transform->getParent() != nullptr)
 		{
			mViewMatrix = glm::inverse(mViewMatrix);
 		}

		shader->setProjectionMatrix(mProjectionMatrix);
		shader->setViewMatrix(mViewMatrix);
		shader->setViewPosition(this->getTransform()->getPosition());
		shader->setMat4(ShaderParam::MatrixSBV, glm::value_ptr(glm::mat4(glm::mat3(mViewMatrix))));
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

		if (mYaw > 360.0f)
		{
			mYaw -= 360.0f;
		}
		else if (mYaw < -360.0f)
		{
			mYaw += 360.0f;
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
		glm::vec3 front;
		front.x = glm::cos(glm::radians(mPitch)) * glm::cos(glm::radians(mYaw));
		front.y = glm::sin(glm::radians(mPitch));
		front.z = glm::cos(glm::radians(mPitch)) * glm::sin(glm::radians(mYaw));
		mFront = glm::normalize(front);

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
		return mViewMatrix;
	}

}
