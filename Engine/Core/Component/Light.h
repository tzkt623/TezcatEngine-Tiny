#pragma once
#include "../Renderer/RenderObject.h"

#include "../Head/CppHead.h"
#include "../Head/GLMHead.h"
#include "../Shader/Shader.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	enum class TINY_API LightType
	{
		Directional,
		Point,
		Spot
	};

	class Shader;
	class TINY_API ILight
	{
	public:
		virtual ~ILight() = default;
		virtual LightType getLightType() = 0;
		virtual void submit(Shader* shader) = 0;
		virtual bool cullGameObject(GameObject* gameObject) = 0;
	};

// 	class TINY_API Light : public ComponentT<Light>, public ILight
// 	{
// 	public:
// 		Light() = default;
// 		virtual ~Light() = default;
// 	};

	class TINY_API DirectionalLight : public ComponentT<DirectionalLight>, public ILight
	{
	public:
		DirectionalLight();
		DirectionalLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);
		virtual ~DirectionalLight();

		LightType getLightType() final { return LightType::Directional; }

		void onEnable() override;
		void onDisable() override;

	public:
		virtual bool cullGameObject(GameObject* gameObject) { return true; }
		void submit(Shader* shader) override;

		glm::vec3& getDirection() { return m_Direction; }
		void setDirection(const glm::vec3& val) { m_Direction = val; }

		glm::vec3& getAmbient() { return m_Ambient; }
		void setAmbient(const glm::vec3& val) { m_Ambient = val; }

		glm::vec3& getDiffuse() { return m_Diffuse; }
		void setDiffuse(const glm::vec3& val) { m_Diffuse = val; }

		glm::vec3& getSpecular() { return m_Specular; }
		void setSpecular(const glm::vec3& val) { m_Specular = val; }

	private:
		glm::vec3 m_Direction;
		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
	};


	class TINY_API PointLight : public ComponentT<PointLight>, public ILight
	{
	public:
		PointLight();
		virtual ~PointLight();

		LightType getLightType() final { return LightType::Point; }

		glm::vec3& getAmbient() { return m_Ambient; }
		void setAmbient(const glm::vec3& val) { m_Ambient = val; }

		glm::vec3& getDiffuse() { return m_Diffuse; }
		void setDiffuse(const glm::vec3& val) { m_Diffuse = val; }

		glm::vec3& getSpecular() { return m_Specular; }
		void setSpecular(const glm::vec3& val) { m_Specular = val; }

		glm::vec3& getConfig() { return m_Specular; }
		void setConfig(const glm::vec3& val) { m_Specular = val; }

		void setConfig(float constant, float linear, float quadratic)
		{
			m_Specular.x = constant;
			m_Specular.y = linear;
			m_Specular.z = quadratic;
		}

		void setConstant(float constant)
		{
			m_Specular.x = constant;
		}

		void setLinear(float linear)
		{
			m_Specular.y = linear;
		}

		void setQuadratic(float quadratic)
		{
			m_Specular.z = quadratic;
		}

		void submit(Shader* shader) override;
		void onEnable() override;
		void onDisable() override;
		bool cullGameObject(GameObject* gameObject) override;

	private:
		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
		glm::vec3 m_Config;
	};

	class TINY_API SpotLight : public ComponentT<SpotLight>, public ILight
	{
	public:
		SpotLight();
		virtual ~SpotLight();

		LightType getLightType() final { return LightType::Spot; }
		bool cullGameObject(GameObject* gameObject) override { return true; }

	private:

	};
}
