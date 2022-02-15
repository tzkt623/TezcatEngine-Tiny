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
#include "../Scene/Layer.h"
#include "../Renderer/BaseGraphics.h"
#include "../Scene/GameObject.h"
#include "../Scene/LayerMask.h"

#include "Utility/Tools.h"


namespace tezcat::Tiny::Core
{
	Camera::Camera(bool mainCamera)
		: m_UID(Utility::IDGenerator<Camera, unsigned int>::generate())
		, m_IsMain(mainCamera)
		, m_NearFace(0.1f)
		, m_FarFace(100.0f)
		, m_FOV(60.0f)
		, m_ProjectionMatrix(1.0f)
		, m_ViewMatrix(1.0f)
		, m_Type(Type::Perspective)
		, m_PMatDirty(true)
		, m_CullMask(1)
		, m_Deep(0)
		, m_Front(0.0f, 0.0f, -1.0f)
		, m_Up(0.0f, 1.0f, 0.0f)
		, m_Right(1.0f, 0.0f, 0.0f)
		, m_WorldUp(0.0f, 1.0f, 0.0f)
		, m_Yaw(-90.0f)
		, m_Pitch(0.0f)
		, m_Roll(0.0f)
		, m_ViewInfo({ 0, 0, Engine::getScreenWidth(), Engine::getScreenHeight() })
	{
		this->setCullLayerMask(LayerMask::getMask(0));
	}

	Camera::~Camera()
	{

	}

	void Camera::setOrtho(float near, float far)
	{
		m_Type = Type::Ortho;
		m_NearFace = near;
		m_FarFace = far;
		m_PMatDirty = true;
	}

	void Camera::setPerspective(float fov, float near, float far)
	{
		m_Type = Type::Perspective;
		m_FOV = fov;
		m_NearFace = near;
		m_FarFace = far;
		m_PMatDirty = true;
	}

	bool Camera::cullGameObject(GameObject* gameObject)
	{
		//#Cull Function
		return true;
	}

	void Camera::onStart()
	{

	}

	void Camera::refreshCullMask()
	{
		m_CullLayerList.clear();
		for (int i = 0; i < 32; i++)
		{
			if (m_CullMask & (1u << i))
			{
				m_CullLayerList.emplace_back(1u << i);
			}
		}
	}

	void Camera::setMain()
	{
		m_IsMain = true;
		CameraMgr::getInstance()->setMainCamera(this);
	}

	void Camera::setViewRect(int x, int y, int width, int height)
	{
		m_ViewInfo.OX = x;
		m_ViewInfo.OY = y;
		m_ViewInfo.Width = width;
		m_ViewInfo.Height = height;
	}

	void Camera::yawPitch(float yaw, float pitch, bool constrainPitch)
	{
		yaw *= 0.2f;
		pitch *= 0.2f;

		m_Yaw += yaw;
		m_Pitch += pitch;

		if (m_Yaw > 360)
		{
			m_Yaw -= 360;
		}
		else if (m_Yaw < -360)
		{
			m_Yaw += 360;
		}

		if (constrainPitch)
		{
			if (m_Pitch > 89.0f)
			{
				m_Pitch = 89.0f;
			}
			if (m_Pitch < -89.0f)
			{
				m_Pitch = -89.0f;
			}
		}

		this->getTransform()->setRotation(m_Pitch, m_Yaw, m_Roll);

		this->updateCameraVector();
	}

	void Camera::updateCameraVector()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Front = glm::normalize(front);

		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	}

	void Camera::roll(float speed)
	{
		m_Roll += speed;
		m_WorldUp = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f) * glm::angleAxis(glm::radians(m_Roll), m_Front));
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	}

	void Camera::render()
	{
		this->update();
		Graphics->cullWithCamera(this);
	}

	void Camera::update()
	{
		if (m_PMatDirty)
		{
			m_PMatDirty = false;
			switch (m_Type)
			{
			case Type::Ortho:
				m_ProjectionMatrix = glm::ortho(
					0.0f, (float)Core::Engine::getScreenWidth(),
					0.0f, (float)Core::Engine::getScreenHeight(),
					m_NearFace, m_FarFace);
				break;
			case Type::Perspective:
				m_ProjectionMatrix = glm::perspective(
					glm::radians(m_FOV),
					(float)Core::Engine::getScreenWidth() / (float)Core::Engine::getScreenHeight(),
					m_NearFace, m_FarFace);
				break;
			default:
				break;
			}
		}

		auto position = this->getTransform()->getPosition();
		m_ViewMatrix = glm::lookAt(position, position + m_Front, m_Up);

		if (this->getTransform()->getParent() != nullptr)
		{
			m_ViewMatrix = this->getTransform()->getParent()->getModelMatrix() * m_ViewMatrix;
		}
	}

	void Camera::onEnable()
	{
		CameraMgr::getInstance()->addCamera(this);
	}

	std::vector<Camera*> CameraLayer::m_Layers[32] = { std::vector<Camera*>() };
	void CameraLayer::addCamera(Camera* camera)
	{
		for (int i = 0; i < 32; i++)
		{
			if (camera->cullLayerMask(1u << i))
			{
				m_Layers[i].push_back(camera);
				return;
			}
		}
	}

	void CameraLayer::removeCamera(Camera* camera)
	{
		for (int i = 0; i < 32; i++)
		{
			if (camera->cullLayerMask(1u << i))
			{
				auto it = std::remove(m_Layers[i].begin(), m_Layers[i].end(), camera);
				m_Layers[i].erase(it);
			}
		}
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
