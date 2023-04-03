#include "ShaderManager.h"
#include "../Shader/Shader.h"
#include "../Shader/ShaderPackage.h"

namespace tezcat::Tiny::Core
{
	ShaderManager::ShaderManager()
	{
		SG<ShaderManager>::attach(this);
	}

	ShaderManager::~ShaderManager()
	{

	}

	int ShaderManager::giveUID()
	{
		int uid = static_cast<int>(m_ShaderPackageWithID.size());
		m_ShaderPackageWithID.push_back(nullptr);
		return uid;
	}

	void ShaderManager::addShader(Shader* shader)
	{
		while (m_AllShaderList.size() <= shader->getUID())
		{
			m_AllShaderList.push_back(nullptr);
		}
		m_AllShaderList[shader->getUID()] = shader;
	}

	void ShaderManager::addShaderPackage(ShaderPackage* shaderPackage)
	{
		if (m_ShaderPackageWithID[shaderPackage->getUID()] == nullptr)
		{
			m_ShaderPackageWithID[shaderPackage->getUID()] = shaderPackage;
			m_ShaderPackageWithName.emplace(shaderPackage->getName(), shaderPackage);
		}
		else
		{
			throw std::invalid_argument(StringTool::stringFormat("ShaderManager: This ShaderPackage[%s] has stored[%d]",
				shaderPackage->getName().c_str(), shaderPackage->getUID()));
		}
	}

	ShaderPackage* ShaderManager::findPackage(const std::string& name)
	{
		auto it = m_ShaderPackageWithName.find(name);
		if (it != m_ShaderPackageWithName.end())
		{
			return it->second;
		}

		return nullptr;
	}
}

