#pragma once
#include "CommonHead.h"

namespace tezcat::Tiny::Module
{
	class Transform
	{
	public:
		Transform();
		Transform(Transform* parent);
		~Transform();

	public:
		glm::vec3& getPosition() { return m_Position; }
		void setPosition(glm::vec3 val)
		{
			m_IsDirty = true;
			m_Position = val;
		}

		void setPosition(float val[])
		{
			m_IsDirty = true;
			m_Position.x = val[0];
			m_Position.y = val[1];
			m_Position.z = val[2];
		}

		glm::vec3& getRotation() { return m_Rotation; }
		void setRotation(glm::vec3 val)
		{
			m_IsDirty = true;
			m_Rotation = val;
		}

		glm::vec3& getScale() { return m_Scale; }
		void setScale(glm::vec3 val)
		{
			m_IsDirty = true;
			m_Scale = val;
		}

		glm::mat4x4& getModelMatrix() { return m_ModelMatrix; }

		Transform* getParent() const { return m_Parent; }
		void setParent(Transform* val) { m_Parent = val; }

	public:
		void update();


	public:
		void translate(const glm::vec3& offset)
		{
			m_IsDirty = true;
			m_Position += offset;
		}

	private:
		Transform* m_Parent;

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;

	private:
		glm::mat4x4 m_ModelMatrix;
		bool m_IsDirty;

	private:
		static glm::vec3 XAxis;
		static glm::vec3 YAxis;
		static glm::vec3 ZAxis;
	};
}