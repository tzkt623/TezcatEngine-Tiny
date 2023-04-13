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
		std::unordered_map<std::string, ShaderPackage*> mShaderPackageDict;
		std::vector<ShaderPackage*> mShaderPackageAry;
		std::vector<Shader*> mAllShaderList;
	};

	using ShaderMgr = SG<ShaderManager>;
}

