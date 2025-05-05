/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Core/Component/Light.h"
#include "Core/Component/Transform.h"
#include "Core/Component/GameObject.h"

#include "Core/Shader/Shader.h"
#include "Core/Renderer/BaseGraphics.h"

#include "Core/Manager/LightingManager.h"


namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(ILight, BaseRenderer);

	TINY_OBJECT_CPP(DirectionalLightAgent, ILight);
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

	void DirectionalLightAgent::makeRenderCommand(PipelineQueue* queue)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void DirectionalLightAgent::makeRenderCommand(ReplacedPipelinePass* pass)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	TINY_OBJECT_CPP(LightComponent, ComponentAutoID<LightComponent>);

		//------------------------------------------------------------------------
	TINY_OBJECT_CPP(DirectionalLight, LightComponent);
	DirectionalLight::DirectionalLight()
		: mLightAgent(DirectionalLightAgent::create())
	{
		mLightAgent->saveObject();
	}

	DirectionalLight::DirectionalLight(const float3& direction, const float3& ambient, const float3& diffuse, const float3& specular)
		: mLightAgent(DirectionalLightAgent::create())
	{
		mLightAgent->saveObject();
	}

	DirectionalLight::~DirectionalLight()
	{
		mLightAgent->deleteObject();
	}

	void DirectionalLight::submit(Shader* shader)
	{
		auto transform = this->getTransform();
		mLightAgent->setDirection(glm::normalize(transform->getForward()));
		mLightAgent->submit(shader);
	}

	void DirectionalLight::onEnable()
	{
		if (mGameObject->getScene())
		{
			LightingManager::setDirectionalLight(this);
		}
	}

	void DirectionalLight::onDisable()
	{
		LightingManager::setDirectionalLight(nullptr);
	}

	void DirectionalLight::render(BaseGraphics* graphics)
	{

	}

	void DirectionalLight::onStart()
	{
		LightingManager::setDirectionalLight(this);
	}

	//------------------------------------------------------------------------
	TINY_OBJECT_CPP(PointLight, LightComponent);
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
	TINY_OBJECT_CPP(SpotLight, LightComponent);
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
