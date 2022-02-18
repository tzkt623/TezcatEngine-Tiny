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
	/// 只需要提供数据
	/// 并不需要自身执行渲染
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

		glm::vec3 getColor() const { return m_Color; }
		void setColor(const glm::vec3& val) { m_Color = val; }

	private:
		glm::vec3 m_Color;
	};


	class DirectionalLight : public Light<DirectionalLight>
	{
	public:
		DirectionalLight();
		DirectionalLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);
		virtual ~DirectionalLight();

		LightType getLightType() final { return LightType::Directional; }

		void sendDataToGPU() override;

		void onEnable() override;

	public:
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


	class PointLight : public Light<PointLight>
	{
	public:
		PointLight();
		~PointLight();

		LightType getLightType() final { return LightType::Point; }

	};

	class Spotlight : public Light<Spotlight>
	{
	public:
		Spotlight();
		~Spotlight();

		LightType getLightType() final { return LightType::Spot; }

	private:

	};
}