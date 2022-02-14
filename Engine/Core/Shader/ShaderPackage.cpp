#include "ShaderPackage.h"
#include "ShaderBuilder.h"
#include "Shader.h"

#include "Utility/Tools.h"
#include "../Manager/ShaderManager.h"

namespace tezcat::Tiny::Core
{
	ShaderPackage::ShaderPackage()
		: m_UID(-1)
	{

	}


	ShaderPackage::~ShaderPackage()
	{

	}

	void ShaderPackage::setConfig(std::unordered_map<std::string, Any>& data)
	{
		m_Name = data["Name"].cast<std::string>();
	}

	void ShaderPackage::addShader(Shader* shader)
	{
		m_Shaders.push_back(shader);
	}

	void ShaderPackage::apply()
	{
		m_UID = SG<ShaderManager>::getInstance()->giveUID();

		if (m_Shaders.size() > 1)
		{
			std::sort(m_Shaders.begin(), m_Shaders.end(),
				[](Shader* a, Shader* b)
				{
					return a->getOrderID() < b->getOrderID();
				});
		}

		SG<ShaderManager>::getInstance()->addShaderPackage(this);
	}
}