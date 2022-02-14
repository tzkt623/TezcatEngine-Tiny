#include "Light.h"
#include "../Shader/Shader.h"


namespace tezcat::Tiny::Core
{
	//------------------------------------------------------------------------
	DirectionalLight::DirectionalLight()
		: m_Direction(0.0f, -1.0f, -1.0f)
		, m_Ambient(1.0f)
		, m_Diffuse(1.0f)
		, m_Specular(1.0f)
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