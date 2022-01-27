#include "Camera.h"
#include "Transform.h"
#include "Tools.h"
#include "Scene.h"
#include "CameraManager.h"
#include "glm/gtx/quaternion.hpp"
#include "glm/ext/quaternion_float.hpp"

namespace tezcat::Tiny::Module
{
	Camera::Camera(const std::string& name) :
		GameObject(name),
		m_NearFace(0.1f),
		m_FarFace(100.0f),
		m_FOV(60.0f),
		m_ProjectionMatrix(1.0f),
		m_ViewMatrix(1.0f),
		m_Type(Type::Perspective),
		m_PMatDirty(true),
		m_Deep(0),
		m_ID(Utility::IDGenerator<Camera, unsigned int>::generate()),
		m_Front(0.0f, 0.0f, -1.0f),
		m_Up(0.0f, 1.0f, 0.0f),
		m_Yaw(-90.0f),
		m_Pitch(0.0f),
		m_Right(1.0f, 0.0f, 0.0f),
		m_WorldUp(0.0f, 1.0f, 0.0f)
	{

	}

	Camera::~Camera()
	{

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

		auto transform = this->getTransform();
		auto position = transform->getPosition();
		m_ViewMatrix = glm::lookAt(position, position + m_Front, m_Up);

		if (transform->getParent() != nullptr)
		{
			m_ViewMatrix = transform->getModelMatrix() * m_ViewMatrix;
		}
	}

	void Camera::sceneEnter(Scene* scene)
	{
		scene->addCamera(this);
		this->updateCameraVector();
		GameObject::sceneEnter(scene);
	}

	void Camera::rotateCamera(float offsetX, float offsetY, bool constrainPitch)
	{
		offsetX *= 0.2f;
		offsetY *= 0.2f;

		m_Yaw += offsetX;
		m_Pitch += offsetY;

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

		this->getTransform()->setRotation(glm::vec3(m_Pitch, m_Yaw, 0.0f));
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
}
