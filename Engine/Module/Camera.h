#pragma once
#include "GameObject.h"
#include "Engine.h"
#include "Matrix.h"

namespace tezcat::Tiny::Module
{
	class Camera : public GameObject
	{
	public:
		Camera(const std::string& name);
		~Camera();

	public:
		enum class Type
		{
			Ortho,
			Perspective
		};

		// ortho
	public:
		void setOrtho(float near, float far)
		{
			m_Type = Type::Ortho;
			m_NearFace = near;
			m_FarFace = far;
			m_PMatDirty = true;
		}

		void setPerspective(float fov, float near, float far)
		{
			m_Type = Type::Perspective;
			m_FOV = fov;
			m_NearFace = near;
			m_FarFace = far;
			m_PMatDirty = true;
		}

		glm::mat4x4& getProjectionMatrix() { return m_ProjectionMatrix; }
		glm::mat4x4& getViewMatrix() { return m_ViewMatrix; }

		glm::vec3& getFront() { return m_Front; }
		glm::vec3& getUp() { return m_Up; }


		int getDeep() const { return m_Deep; }
		void setDeep(int val) { m_Deep = val; }

		unsigned int getID() const { return m_ID; }

		void translateYP(float yaw, float pitch)
		{
			m_Yaw += yaw;
			m_Pitch += pitch;
		}

	public:
		void update() override;
		void sceneEnter(Scene* scene) override;

	private:
		void updateCameraVector();

	private:
		int m_Deep;
		unsigned int m_ID;
	private:
		float m_NearFace;
		float m_FarFace;
		float m_FOV;
		Type m_Type;
		bool m_PMatDirty;

		glm::mat4x4 m_ProjectionMatrix;
		glm::mat4x4 m_ViewMatrix;

	private:
		glm::vec3 m_Front;
		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_WorldUp;
		float m_Yaw;
		float m_Pitch;
	};
}
