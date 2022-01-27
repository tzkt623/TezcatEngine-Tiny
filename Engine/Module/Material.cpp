#include "Material.h"
#include "Engine.h"
#include "ShaderManager.h"
#include "Texture.h"
#include "Tools.h"

namespace tezcat::Tiny::Module
{
	int Material::IDGiver = 0;

	Material::Material(const std::string& name) :
		m_Name(name),
		m_ID(Utility::IDGenerator<Material, unsigned int>::generate()),
		m_Shader(nullptr)
	{
		m_Shader = Core::Engine::getInstance()->getShaderManager()->find(name);
	}

	Material::~Material()
	{
		m_TextureWithName.clear();
		m_Shader = nullptr;
	}

	void Material::addTexture(const std::string& texType, Core::Texture* texture)
	{
		m_TextureWithName[texType] = texture;
	}
}
