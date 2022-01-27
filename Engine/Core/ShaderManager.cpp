#include "ShaderManager.h"
#include "Shader.h"

namespace tezcat::Tiny::Core
{
	IMPLEMENT_DELEGATE_INSTANCE(ShaderManager);
	ShaderManager::ShaderManager()
	{
		REGISTER_DELEGATE_INSTANCE(ShaderManager);
	}

	ShaderManager::~ShaderManager()
	{
	}

	void ShaderManager::addShader(Shader* shader)
	{
		m_ShaderMap.emplace(shader->getName(), shader);
	}

	Shader* ShaderManager::find(const std::string& name)
	{
		auto it = m_ShaderMap.find(name);
		if (it != m_ShaderMap.end())
		{
			return it->second;
		}

		return nullptr;
	}

}

