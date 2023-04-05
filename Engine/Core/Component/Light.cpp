#include "Light.h"
#include "Transform.h"

#include "../Shader/Shader.h"
#include "../Manager/LightManager.h"
#include "../Component/GameObject.h"


namespace tezcat::Tiny::Core
{
	//------------------------------------------------------------------------
	DirectionalLight::DirectionalLight()
		: mDirection(-1.0f, -1.0f, -1.0f)
		, mAmbient(0.1f)
		, mDiffuse(1.0f)
		, mSpecular(0.2f)
	{

	}

	DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
		: mDirection(direction)
		, mAmbient(ambient)
		, mDiffuse(diffuse)
		, mSpecular(specular)
	{

	}

	DirectionalLight::~DirectionalLight()
	{

	}

	void DirectionalLight::submit(Shader* shader)
	{
		shader->setFloat3(ShaderParam::LightDirection::Direction, glm::value_ptr(mDirection));
		shader->setFloat3(ShaderParam::LightDirection::Ambient, glm::value_ptr(mAmbient));
		shader->setFloat3(ShaderParam::LightDirection::Diffuse, glm::value_ptr(mDiffuse));
		shader->setFloat3(ShaderParam::LightDirection::Specular, glm::value_ptr(mSpecular));
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
		: mAmbient(0.2f)
		, mDiffuse(0.5f)
		, mSpecular(1.0f)
		, mConfig(1.0f, 0.09f, 0.032f)
	{

	}

	PointLight::~PointLight()
	{

	}

	void PointLight::submit(Shader* shader)
	{
		shader->setFloat3(ShaderParam::LightPoint::Position, glm::value_ptr(this->getTransform()->getPosition()));
		shader->setFloat3(ShaderParam::LightPoint::Ambient, glm::value_ptr(mAmbient));
		shader->setFloat3(ShaderParam::LightPoint::Diffuse, glm::value_ptr(mDiffuse));
		shader->setFloat3(ShaderParam::LightPoint::Specular, glm::value_ptr(mSpecular));
		shader->setFloat3(ShaderParam::LightPoint::Config, glm::value_ptr(mConfig));
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
