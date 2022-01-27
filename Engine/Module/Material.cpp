#include "Material.h"
#include "Engine.h"
#include "ShaderManager.h"

namespace tezcat::Tiny::Module
{
	int Material::IDGiver = 0;

	Material::Material(const std::string& name) :
		m_Name(name),
		m_ID(IDGiver++),
		m_Shader(nullptr)
	{
		m_Shader = Core::Engine::getInstance()->getShaderManager()->find(name);
	}

	Material::~Material()
	{
		m_Shader = nullptr;
	}
}
