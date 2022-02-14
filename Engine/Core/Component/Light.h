#pragma once
#include "RenderObject.h"

#include "../Head/CppHead.h"
#include "../Head/GLMHead.h"
#include "../Shader/Shader.h"

namespace tezcat::Tiny::Core
{
	enum class LightType
	{
		Directional,
		Point,
		Spot
	};

	class Shader;
	/// <summary>
	/// ֻ��Ҫ�ṩ����
	/// ������Ҫ����ִ����Ⱦ
	/// </summary>
	template<typename T>
	class Light : public RenderObject<T>
	{
	public:

	public:
		Light() {}
		virtual ~Light() {}
		virtual LightType getLightType() = 0;
		RenderObjectType getRenderObjectType() final { return RenderObjectType::Light; }

		template<typename T>
		void render(Shader* shader)
		{
			shader->setFloat3(m_Name, glm::value_ptr(m_Color));
		}

		glm::vec3 getColor() const { return m_Color; }
		void setColor(const glm::vec3& val) { m_Color = val; }

	private:
		std::string m_Name;
		glm::vec3 m_Color;
	};


	class DirectionalLight : Light<DirectionalLight>
	{
	public:
		DirectionalLight();
		DirectionalLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);
		virtual ~DirectionalLight();

		LightType getLightType() final { return LightType::Directional; }

	public:
		void submit(Shader* shader) override;

		glm::vec3 getDirection() const { return m_Direction; }
		void setDirection(glm::vec3 val) { m_Direction = val; }

		glm::vec3 getAmbient() const { return m_Ambient; }
		void setAmbient(glm::vec3 val) { m_Ambient = val; }

		glm::vec3 getDiffuse() const { return m_Diffuse; }
		void setDiffuse(glm::vec3 val) { m_Diffuse = val; }

		glm::vec3 getSpecular() const { return m_Specular; }
		void setSpecular(glm::vec3 val) { m_Specular = val; }

	private:
		glm::vec3 m_Direction;
		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
	};


	class PointLight : public Light<PointLight>
	{
	public:
		PointLight();
		~PointLight();

		LightType getLightType() final { return LightType::Point; }

	};

	class Spotlight : public  Light<Spotlight>
	{
	public:
		Spotlight();
		~Spotlight();

		LightType getLightType() final { return LightType::Spot; }

	private:

	};
}