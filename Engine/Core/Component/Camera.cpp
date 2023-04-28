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
#include "../Renderer/RenderLayer.h"


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
			transform->getWorldPosition(),
			transform->getWorldPosition() + transform->getForward(),
			transform->getUp());

		shader->setProjectionMatrix(mProjectionMatrix);
		shader->setViewMatrix(mViewMatrix);
		shader->setViewPosition(this->getTransform()->getWorldPosition());
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
}
