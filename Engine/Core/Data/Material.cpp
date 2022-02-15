#include "Material.h"
#include "Utility/Tools.h"

#include "../Manager/ShaderManager.h"
#include "../Shader/ShaderPackage.h"
#include "../Head/ContextMap.h"
#include "../Renderer/BaseGraphics.h"

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

	void Material::addTexture(const std::string& texType, Texture* texture)
	{
		m_TextureWithName[texType] = texture;
	}

	Texture* Material::addTexture2D(const std::string& texType, const std::string& filePath)
	{
		auto t2d = Graphics->createTexture(filePath, TextureType::Texture2D);
		m_TextureWithName[texType] = t2d;
		return t2d;
	}
}
