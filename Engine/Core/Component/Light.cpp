#include "Light.h"
#include "Transform.h"

#include "../Shader/Shader.h"
#include "../Manager/LightManager.h"
#include "../Component/GameObject.h"
#include "../Renderer/BaseGraphics.h"


namespace tezcat::Tiny
{
	//------------------------------------------------------------------------
	TINY_RTTI_CPP(DirectionalLight);
	DirectionalLight::DirectionalLight()
		: mDirection(0.0f, 0.0f, 0.0f)
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

	void DirectionalLight::submit(BaseGraphics* graphics, Shader* shader)
	{
		auto transform = this->getTransform();
		mDirection = glm::normalize(transform->getForward());
		graphics->setFloat3(shader, ShaderParam::LightDirection::Direction, glm::value_ptr(mDirection));
		graphics->setFloat3(shader, ShaderParam::LightDirection::Ambient, glm::value_ptr(mAmbient));
		graphics->setFloat3(shader, ShaderParam::LightDirection::Diffuse, glm::value_ptr(mDiffuse));
		graphics->setFloat3(shader, ShaderParam::LightDirection::Specular, glm::value_ptr(mSpecular));
	}

	void DirectionalLight::onEnable()
	{
		LightMgr::getInstance()->setDirectionalLight(this);
	}

	void DirectionalLight::onDisable()
	{

	}

	void DirectionalLight::render(BaseGraphics* graphics)
	{

	}

	//------------------------------------------------------------------------
	TINY_RTTI_CPP(PointLight);
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

	void PointLight::submit(BaseGraphics* graphics, Shader* shader)
	{
		graphics->setFloat3(shader, ShaderParam::LightPoint::Position, glm::value_ptr(this->getTransform()->getPosition()));
		graphics->setFloat3(shader, ShaderParam::LightPoint::Ambient, glm::value_ptr(mAmbient));
		graphics->setFloat3(shader, ShaderParam::LightPoint::Diffuse, glm::value_ptr(mDiffuse));
		graphics->setFloat3(shader, ShaderParam::LightPoint::Specular, glm::value_ptr(mSpecular));
		graphics->setFloat3(shader, ShaderParam::LightPoint::Config, glm::value_ptr(mConfig));
	}

	void PointLight::onEnable()
	{
		LightMgr::getInstance()->addPointLight(this);
	}

	void PointLight::onDisable()
	{

	}

	//------------------------------------------------------------------------
	TINY_RTTI_CPP(SpotLight);
	SpotLight::SpotLight()
	{

	}

	SpotLight::~SpotLight()
	{

	}

}
