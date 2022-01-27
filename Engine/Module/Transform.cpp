#include "Transform.h"
#include "glm/gtx/quaternion.hpp"

namespace tezcat::Tiny::Module
{
	glm::vec3 Transform::XAxis(1.0f, 0.0f, 0.0f);
	glm::vec3 Transform::YAxis(0.0f, 1.0f, 0.0f);
	glm::vec3 Transform::ZAxis(0.0f, 0.0f, 1.0f);

	Transform::Transform() :
		Transform(nullptr)
	{

	}

	Transform::Transform(Transform* parent) :
		m_IsDirty(true),
		m_Position(0.0f),
		m_Rotation(0.0f),
		m_Scale(1.0f),
		m_ModelMatrix(1.0f),
		m_Parent(parent)
	{

	}

	Transform::~Transform()
	{

	}

	void Transform::update()
	{
		if (m_IsDirty)
		{
			m_IsDirty = false;

			if (m_Parent == nullptr)
			{
				m_ModelMatrix = glm::mat4x4(1.0f);
			}
			else
			{
				m_ModelMatrix = m_Parent->getModelMatrix();
			}

			m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
// 			m_ModelMatrix = glm::rotate(m_ModelMatrix, m_Rotation.x, XAxis);
// 			m_ModelMatrix = glm::rotate(m_ModelMatrix, m_Rotation.y, YAxis);
// 			m_ModelMatrix = glm::rotate(m_ModelMatrix, m_Rotation.z, ZAxis);

			m_ModelMatrix *= glm::toMat4(glm::quat(glm::radians(m_Rotation)));
			m_ModelMatrix = glm::scale(m_ModelMatrix, m_Scale);
		}
	}
}