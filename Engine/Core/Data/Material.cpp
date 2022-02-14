#include "Material.h"
#include "../Manager/ShaderManager.h"
#include "../Shader/ShaderPackage.h"
#include "Utility/Tools.h"

namespace tezcat::Tiny::Core
{
	int Material::IDGiver = 0;

	Material::Material(const std::string& name)
		: m_Name(name)
	{
		m_ShaderPackage = ShaderMgr::getInstance()->findPackage(name);
	}

	Material::~Material()
	{
		m_TextureWithName.clear();
		m_ShaderPackage = nullptr;
	}

	int Material::getUID()
	{
		return m_ShaderPackage->getUID();
	}

	void Material::addTexture(const std::string& texType, Core::Texture* texture)
	{
		m_TextureWithName[texType] = texture;
	}
}
