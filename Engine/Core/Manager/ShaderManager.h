#pragma once
#include "../Head/CppHead.h"
#include "Utility/Tools.h"

namespace tezcat::Tiny::Core
{
	class ShaderPackage;
	class ShaderManager
	{
	public:
		ShaderManager();
		~ShaderManager();

	public:
		int giveUID();
		void addShaderPackage(ShaderPackage* shaderPackage);
		ShaderPackage* findPackage(const std::string& name);

	private:
		std::unordered_map<std::string, ShaderPackage*> m_ShaderPackageWithName;
		std::vector<ShaderPackage*> m_ShaderPackageWithID;
	};

	using ShaderMgr = SG<ShaderManager>;
}

