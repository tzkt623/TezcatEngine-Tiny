#include "Light.h"
#include "../Shader/Shader.h"
#include "../Manager/LightManager.h"


namespace tezcat::Tiny::Core
{
	//------------------------------------------------------------------------
	DirectionalLight::DirectionalLight()
		: m_Direction(-1.0f, -1.0f, -1.0f)
		, m_Ambient(0.1f)
		, m_Diffuse(1.0f)
		, m_Specular(0.2f)
	{

	}

	DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
		: m_Direction(direction)
		, m_Ambient(ambient)
		, m_Diffuse(diffuse)
		, m_Specular(specular)
	{

	}

	DirectionalLight::~DirectionalLight()
	{

	}

	void DirectionalLight::submit(Shader* shader)
	{
		shader->setFloat3(ShaderParam::LightDirection::Direction, glm::value_ptr(m_Direction));
		shader->setFloat3(ShaderParam::LightDirection::Ambient, glm::value_ptr(m_Ambient));
		shader->setFloat3(ShaderParam::LightDirection::Diffuse, glm::value_ptr(m_Diffuse));
		shader->setFloat3(ShaderParam::LightDirection::Specular, glm::value_ptr(m_Specular));
	}

	void DirectionalLight::sendDataToGPU()
	{

	}

	void DirectionalLight::onEnable()
	{
		LightMgr::getInstance()->setDirectionalLight(this);
	}

	//------------------------------------------------------------------------


	PointLight::PointLight()
	{

	}

	PointLight::~PointLight()
	{

	}



	//------------------------------------------------------------------------


	Spotlight::Spotlight()
	{

	}

	Spotlight::~Spotlight()
	{

	}
}