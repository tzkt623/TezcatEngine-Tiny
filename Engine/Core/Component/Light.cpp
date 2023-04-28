#include "Light.h"
#include "Transform.h"

#include "../Shader/Shader.h"
#include "../Manager/LightManager.h"
#include "../Component/GameObject.h"
#include "../Renderer/BaseGraphics.h"


namespace tezcat::Tiny
{
	//------------------------------------------------------------------------
	TINY_RTTI_CPP(DirectionalLight)
	DirectionalLight::DirectionalLight()
		: mDirection(0.0f, 0.0f, 0.0f)
		, mAmbient(0.1f)
		, mDiffuse(1.0f)
		, mSpecular(0.2f)
		, mViewMatrix(1.0f)
	{

	}

	DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
		: mDirection(direction)
		, mAmbient(ambient)
		, mDiffuse(diffuse)
		, mSpecular(specular)
		, mViewMatrix(1.0f)
	{

	}

	DirectionalLight::~DirectionalLight()
	{

	}

	void DirectionalLight::submit(Shader* shader)
	{
		auto transform = this->getTransform();
		mDirection = glm::normalize(transform->getForward());
		//std::cout << mDirection[0] << "," << mDirection[1] << "," << mDirection[2] << std::endl;

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

	void DirectionalLight::render(BaseGraphics* graphics)
	{

	}

	void DirectionalLight::submitViewMatrix(Shader* shader)
	{
		this->updateObserverMatrix();
		auto lpv = mProjectionMatrix * this->getViewMatrix();
		shader->setMat4(ShaderParam::MatrixLit, lpv);
	}

	glm::mat4& DirectionalLight::getViewMatrix()
	{
		auto transform = this->getTransform();
		mViewMatrix = glm::lookAt(
			transform->getWorldPosition(),
			transform->getWorldPosition() + transform->getForward(),
			transform->getUp());
		return mViewMatrix;
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

	//------------------------------------------------------------------------
	SpotLight::SpotLight()
	{

	}

	SpotLight::~SpotLight()
	{

	}

}
