#include "Camera.h"
#include "Transform.h"
#include "Component.h"

#include "../Engine.h"
#include "../Manager/CameraManager.h"
#include "../Pipeline/Pipeline.h"
#include "../Component/MeshRenderer.h"
#include "../Scene/Scene.h"
#include "../Data/Material.h"
#include "../Shader/ShaderPackage.h"
#include "../Shader/Shader.h"

#include "../Manager/SceneManager.h"
#include "../Renderer/BaseGraphics.h"
#include "../Component/GameObject.h"
#include "../Scene/LayerMask.h"

#include "Utility/Utility.h"
#include "../Pipeline/Forward.h"
#include "../Manager/PipelineManager.h"


namespace tezcat::Tiny::Core
{
	Camera::Camera(Pipeline* pipeline, bool mainCamera)
		: mUID(Utility::IDGenerator<Camera, unsigned int>::generate())
		, mPipeline(pipeline)
		, mIsMain(mainCamera)
		, mNearFace(0.1f)
		, mFarFace(100.0f)
		, mFOV(60.0f)
		, mAspect(Engine::getScreenWidth() / (float)Engine::getScreenHeight())
		, mProjectionMatrix(1.0f)
		, mViewMatrix(1.0f)
		, mType(Type::Perspective)
		, mPMatDirty(true)
		, mDeep(0)
		, mFront(0.0f, 0.0f, -1.0f)
		, mUp(0.0f, 1.0f, 0.0f)
		, mRight(1.0f, 0.0f, 0.0f)
		, mWorldUp(0.0f, 1.0f, 0.0f)
		, mYaw(-90.0f)
		, mPitch(0.0f)
		, mRoll(0.0f)
		, mViewInfo({ 0, 0, Engine::getScreenWidth(), Engine::getScreenHeight() })
	{

	}

	Camera::Camera(Pipeline* pipeling)
		: Camera(pipeling, true)
	{

	}

	Camera::Camera(bool mainCamera)
		: Camera(PipelineManager::get("Forward"), mainCamera)
	{
	}

	Camera::Camera()
		: Camera(PipelineManager::get("Forward"), true)
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
					0.0f, (float)Core::Engine::getScreenWidth(),
					0.0f, (float)Core::Engine::getScreenHeight(),
					mNearFace, mFarFace);
				break;
			case Type::Perspective:
				mProjectionMatrix = glm::perspective(
					glm::radians(mFOV),
					(float)Core::Engine::getScreenWidth() / (float)Core::Engine::getScreenHeight(),
					mNearFace, mFarFace);
				break;
			default:
				break;
			}
		}

		auto position = this->getTransform()->getPosition();
		mViewMatrix = glm::lookAt(position, position + mFront, mUp);

		if (this->getTransform()->getParent() != nullptr)
		{
			mViewMatrix = this->getTransform()->getParent()->getModelMatrix() * mViewMatrix;
		}
	}

	void Camera::setOrtho(float near, float far)
	{
		mType = Type::Ortho;
		mNearFace = near;
		mFarFace = far;
		mPMatDirty = true;
	}

	void Camera::setPerspective(float fov, float near, float far)
	{
		mType = Type::Perspective;
		mFOV = fov;
		mNearFace = near;
		mFarFace = far;
		mPMatDirty = true;
	}

	void Camera::setMain()
	{
		mIsMain = true;
		CameraMgr::getInstance()->setMainCamera(this);
	}

	void Camera::setViewRect(int x, int y, int width, int height)
	{
		mViewInfo.OX = x;
		mViewInfo.OY = y;
		mViewInfo.Width = width;
		mViewInfo.Height = height;
	}

	void Camera::render()
	{
		this->onUpdate();
		mPipeline->render(this);
	}

	void Camera::submit(Shader* shader)
	{
		shader->setProjectionMatrix(this->getProjectionMatrix());
		shader->setViewMatrix(this->getViewMatrix());
		shader->setViewPosition(this->getTransform()->getPosition());
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

		this->updateCameraVector();
	}

	void Camera::updateCameraVector()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		front.y = sin(glm::radians(mPitch));
		front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		mFront = glm::normalize(front);

		mRight = glm::normalize(glm::cross(mFront, mWorldUp));
		mUp = glm::normalize(glm::cross(mRight, mFront));
	}

	void Camera::roll(float speed)
	{
		mRoll += speed;
		mWorldUp = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f) * glm::angleAxis(glm::radians(mRoll), mFront));
		mRight = glm::normalize(glm::cross(mFront, mWorldUp));
		mUp = glm::normalize(glm::cross(mRight, mFront));
	}

	//----------------------------------------
	//
	//	layer
	//

	std::vector<Camera*> CameraLayer::m_Layers[32] = { std::vector<Camera*>() };
	void CameraLayer::addCamera(Camera* camera)
	{
		// 		for (int i = 0; i < 32; i++)
		// 		{
		// 			if (camera->cullLayerMask(1u << i))
		// 			{
		// 				m_Layers[i].push_back(camera);
		// 				return;
		// 			}
		// 		}
	}

	void CameraLayer::removeCamera(Camera* camera)
	{
		// 		for (int i = 0; i < 32; i++)
		// 		{
		// 			if (camera->cullLayerMask(1u << i))
		// 			{
		// 				auto it = std::remove(m_Layers[i].begin(), m_Layers[i].end(), camera);
		// 				m_Layers[i].erase(it);
		// 			}
		// 		}
	}

	void CameraLayer::addCamera(Camera* camera, unsigned int mask)
	{
		int index = binarySearch(mask, 32);
		m_Layers[index].push_back(camera);
	}

	bool CameraLayer::removeCamera(Camera* camera, unsigned int mask)
	{
		int index = binarySearch(mask, 32);
		if (index != -1)
		{
			auto it = std::remove(m_Layers[index].begin(), m_Layers[index].end(), camera);
			m_Layers[index].erase(it);
			return true;
		}
		return false;
	}

	int CameraLayer::binarySearch(unsigned int mask, int count)
	{
		int min = 0;
		int max = count - 1;

		while (min <= max)
		{
			int mid = min + ((max - min) >> 1);
			if (mask > (1u << mid))
			{
				min = mid + 1;
			}
			else if (mask < (1u << mid))
			{
				max = mid - 1;
			}
			else
			{
				return mid;
			}
		}

		return -1;
	}
}
