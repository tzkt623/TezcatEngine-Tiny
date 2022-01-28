#include "Light.h"
#include "Shader.h"


namespace tezcat::Tiny::Core
{
	Light::Light()
	{

	}

	Light::~Light()
	{

	}

	void Light::render(Shader* shader)
	{
		shader->setFloat3(m_Name, glm::value_ptr(m_Color));
	}

	void Light::apply()
	{

	}
}