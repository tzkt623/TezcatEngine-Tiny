#pragma once
#include "../Head/CppHead.h"
#include "Utility/Utility.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class ShaderPackage;
	class Shader;
	class TINY_API ShaderManager
	{
	public:
		ShaderManager();
		~ShaderManager();

	public:
		int giveUID();

		void addShader(Shader* shader);
		void addShaderPackage(ShaderPackage* shaderPackage);
		ShaderPackage* findPackage(const std::string& name);

	private:
		std::unordered_map<std::string, ShaderPackage*> m_ShaderPackageWithName;
		std::vector<ShaderPackage*> m_ShaderPackageWithID;
		std::vector<Shader*> m_AllShaderList;
	};

	using ShaderMgr = SG<ShaderManager>;
}

