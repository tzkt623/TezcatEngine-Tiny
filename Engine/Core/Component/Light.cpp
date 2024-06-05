#include "Light.h"
#include "Transform.h"

#include "../Shader/Shader.h"
#include "../Component/GameObject.h"
#include "../Renderer/BaseGraphics.h"

#include "../Manager/LightingManager.h"


namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(ILight, BaseRenderer)

	TINY_OBJECT_CPP(DirectionalLightAgent, ILight)
	DirectionalLightAgent::DirectionalLightAgent()
		: mDirection()
		, mAmbient()
		, mDiffuse()
		, mSpecular()
	{

	}
	void DirectionalLightAgent::submit(Shader* shader)
	{
		Graphics::getInstance()->setFloat3(shader, ShaderParam::LightDirection::Direction, glm::value_ptr(mDirection));
		Graphics::getInstance()->setFloat3(shader, ShaderParam::LightDirection::Ambient, glm::value_ptr(mAmbient));
		Graphics::getInstance()->setFloat3(shader, ShaderParam::LightDirection::Diffuse, glm::value_ptr(mDiffuse));
		Graphics::getInstance()->setFloat3(shader, ShaderParam::LightDirection::Specular, glm::value_ptr(mSpecular));
	}

	void DirectionalLightAgent::makeRenderCommand(BaseRenderObserver* renderObserver)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void DirectionalLightAgent::makeRenderCommand(PipelinePass* pass)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	TINY_OBJECT_CPP(LightComponent, ComponentT<LightComponent>)

		//------------------------------------------------------------------------
		TINY_OBJECT_CPP(DirectionalLight, LightComponent)
		DirectionalLight::DirectionalLight()
		: mLightAgent(nullptr)
	{
		mLightAgent = DirectionalLightAgent::create();
		mLightAgent->saveObject();
	}

	DirectionalLight::DirectionalLight(const float3& direction, const float3& ambient, const float3& diffuse, const float3& specular)
		: mLightAgent(nullptr)
	{
		mLightAgent->deleteObject();
	}

	DirectionalLight::~DirectionalLight()
	{

	}

	void DirectionalLight::submit(Shader* shader)
	{
		auto transform = this->getTransform();
		mLightAgent->setDirection(glm::normalize(transform->getForward()));
		mLightAgent->submit(shader);
	}

	void DirectionalLight::onEnable()
	{

	}

	void DirectionalLight::onDisable()
	{

	}

	void DirectionalLight::render(BaseGraphics* graphics)
	{

	}

	void DirectionalLight::onStart()
	{
		LightingManager::setDirectionalLight(this);
	}

	//------------------------------------------------------------------------
	TINY_OBJECT_CPP(PointLight, LightComponent)
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
		Graphics::getInstance()->setFloat3(shader, ShaderParam::LightPoint::Position, glm::value_ptr(this->getTransform()->getPosition()));
		Graphics::getInstance()->setFloat3(shader, ShaderParam::LightPoint::Ambient, glm::value_ptr(mAmbient));
		Graphics::getInstance()->setFloat3(shader, ShaderParam::LightPoint::Diffuse, glm::value_ptr(mDiffuse));
		Graphics::getInstance()->setFloat3(shader, ShaderParam::LightPoint::Specular, glm::value_ptr(mSpecular));
		Graphics::getInstance()->setFloat3(shader, ShaderParam::LightPoint::Config, glm::value_ptr(mConfig));
	}

	void PointLight::onEnable()
	{

	}

	void PointLight::onDisable()
	{

	}

	//------------------------------------------------------------------------
	TINY_OBJECT_CPP(SpotLight, LightComponent)
	SpotLight::SpotLight()
	{

	}

	SpotLight::~SpotLight()
	{

	}

	void SpotLight::submit(Shader* shader)
	{

	}

}
