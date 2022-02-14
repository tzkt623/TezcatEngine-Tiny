#pragma once

#include "../Head/CppHead.h"
#include "../Head/GLMHead.h"
#include "../Head/ConfigHead.h"
#include "Component.h"

namespace tezcat::Tiny::Core
{
	class Camera;
	class TINY_API CameraLayer
	{
	public:
		static void addCamera(Camera* camera);
		static void removeCamera(Camera* camera);

		static void addCamera(Camera* camera, unsigned int mask);
		static bool removeCamera(Camera* camera, unsigned int mask);

	private:
		static int binarySearch(unsigned int mask, int count);

	private:
		static std::vector<Camera*> m_Layers[32];
	};

	class Transform;
	class MeshRenderer;
	class Shader;
	class Scene;
	/// <summary>
	/// 相机只提供矩阵信息
	/// 而且矩阵信息在渲染之前计算
	/// </summary>
	class TINY_API Camera : public ComponentT<Camera>
	{
	public:
		enum class Type
		{
			Ortho,
			Perspective
		};

	public:
		Camera(bool mainCamera = false);
		~Camera();

		void setOrtho(float near, float far);
		void setPerspective(float fov, float near, float far);

		inline Type getViewType() { return m_Type; }
		inline uint32_t getID() const { return m_UID; }

	public:

		inline const glm::mat4x4& getProjectionMatrix() { return m_ProjectionMatrix; }
		inline const glm::mat4x4& getViewMatrix() { return m_ViewMatrix; }

	public:
		inline bool isMain() const { return m_IsMain; }
		void setMain();
		inline void clearMain() { m_IsMain = false; }

		inline int getDeep() const { return m_Deep; }
		inline void setDeep(int val) { m_Deep = val; }

		inline bool cullLayerMask(uint32_t layerMask) { return (m_CullMask & layerMask) > 0; }
		void setCullLayerMask(uint32_t mask)
		{
			m_CullMask = mask;
			this->refreshCullMask();
		}
		void addCullLayerMask(uint32_t mask)
		{
			m_CullMask |= mask;
			this->refreshCullMask();
		}

		void removeCullLayerMask(uint32_t mask)
		{
			m_CullMask &= ~mask;
			this->refreshCullMask();
		}

		bool cullGameObject(GameObject* gameObject);

		std::vector<glm::uint32_t> getCullLayerList() const { return m_CullLayerList; }


	protected:
		void onStart() override;
		void onEnable() override;

	public:
		inline glm::vec3& getFront() { return m_Front; }
		inline glm::vec3& getUp() { return m_Up; }
		inline glm::vec3& getRight() { return m_Right; }

		void setViewrect(int x, int y, int width, int height);
		void updateViewport();

	public:
		void yawPitch(float yaw, float pitch, bool constrainPitch = true);
		void roll(float roll);
		virtual void render();
		void update();


	protected:
		void refreshCullMask();

	private:
		void updateCameraVector();

	private:
		int m_Deep;
		bool m_IsMain;

		uint32_t m_UID;
		uint32_t m_CullMask;

	private:
		int m_ViewX;
		int m_ViewY;
		int m_ViewWidth;
		int m_ViewHeight;

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
		float m_Roll;

		std::vector<uint32_t> m_CullLayerList;
	};
}
