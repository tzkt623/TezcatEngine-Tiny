#include "Material.h"
#include "Utility/Tools.h"

#include "../Manager/ShaderManager.h"
#include "../Shader/ShaderPackage.h"
#include "../Head/Context.h"
#include "../Renderer/BaseGraphics.h"
#include "../Shader/Shader.h"
#include "../Component/Transform.h"

namespace tezcat::Tiny::Core
{
	Material::Material(const std::string& name)
		: m_Name(name)
	{
		m_ShaderPackage = ShaderMgr::getInstance()->findPackage(name);
	}

	Material::~Material()
	{
		m_Uniforms.clear();
		m_ShaderPackage = nullptr;
	}

	int Material::getUID()
	{
		return m_ShaderPackage->getUID();
	}

	void Material::submit(Transform* transform, Shader* shader)
	{
		shader->resetState();
		for (const auto& uniform : m_Uniforms)
		{
			uniform->submit(transform, shader);
		}
	}
}
