#include "Camera.h"
#include "Transform.h"
#include "Component.h"

#include "../Engine.h"

#include "../Scene/Scene.h"

#include "../Manager/CameraManager.h"
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
#include "../Renderer/RenderLayer.h"
#include "../Renderer/RenderPass.h"

#include "../Data/Material.h"

#include "../Tool/Tool.h"


namespace tezcat::Tiny
{
	TINY_RTTI_CPP(Camera);

	Camera::Camera(bool mainCamera)
		: IRenderObserver(new BaseQueue(this))
		, mUID(IDGenerator<Camera, unsigned int>::generate())
		, mIsMain(mainCamera)
	{
		mClearOption = ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth);
	}

	Camera::Camera()
		: Camera(true)
	{

	}

	Camera::~Camera()
	{

	}

	void Camera::onStart()
	{
	}

	void Camera::onEnable()
	{
		CameraMgr::getInstance()->addCamera(this);
	}

	void Camera::onDisable()
	{

	}

	void Camera::setMain()
	{
		mIsMain = true;
		CameraMgr::getInstance()->setMainCamera(this);
	}

	void Camera::render(BaseGraphics* graphics)
	{

	}

	void Camera::submit(Shader* shader)
	{

	}

	void Camera::submitViewMatrix(Shader* shader)
	{
		this->updateObserverMatrix();
		auto transform = this->getTransform();

		mViewMatrix = glm::lookAt(transform->getWorldPosition()
								, transform->getWorldPosition() + transform->getForward()
								, transform->getUp());

		shader->setProjectionMatrix(mProjectionMatrix);
		shader->setViewMatrix(mViewMatrix);
		shader->setViewPosition(this->getTransform()->getWorldPosition());
		shader->setMat4(ShaderParam::MatrixSBV, glm::value_ptr(glm::mat4(glm::mat3(mViewMatrix))));
		shader->setFloat2(ShaderParam::ViewNearFar, glm::vec2(mNearFace, mFarFace));
	}
}
