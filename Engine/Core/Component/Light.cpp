#include "Light.h"
#include "Transform.h"

#include "../Shader/Shader.h"
#include "../Manager/LightManager.h"
#include "../Component/GameObject.h"


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

	void DirectionalLight::onEnable()
	{
		LightMgr::getInstance()->setDirectionalLight(this);
	}

	void DirectionalLight::onDisable()
	{

	}

	//------------------------------------------------------------------------
	PointLight::PointLight()
		: m_Ambient(0.2f)
		, m_Diffuse(0.5f)
		, m_Specular(1.0f)
		, m_Config(1.0f, 0.09f, 0.032f)
	{

	}

	PointLight::~PointLight()
	{

	}

	void PointLight::submit(Shader* shader)
	{
		shader->setFloat3(ShaderParam::LightPoint::Position, glm::value_ptr(this->getTransform()->getPosition()));
		shader->setFloat3(ShaderParam::LightPoint::Ambient, glm::value_ptr(m_Ambient));
		shader->setFloat3(ShaderParam::LightPoint::Diffuse, glm::value_ptr(m_Diffuse));
		shader->setFloat3(ShaderParam::LightPoint::Specular, glm::value_ptr(m_Specular));
		shader->setFloat3(ShaderParam::LightPoint::Config, glm::value_ptr(m_Config));
	}

	void PointLight::onEnable()
	{
		LightMgr::getInstance()->addPointLight(this);
	}

	void PointLight::onDisable()
	{
		
	}

	bool PointLight::cullGameObject(GameObject* gameObject)
	{
		gameObject->getTransform()->getPosition();
		return false;
	}

	//------------------------------------------------------------------------
	SpotLight::SpotLight()
	{

	}

	SpotLight::~SpotLight()
	{

	}
}
