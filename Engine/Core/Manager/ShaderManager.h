#pragma once
#include "../Head/CppHead.h"
#include "Utility/Utility.h"
#include "../Head/ConfigHead.h"
#include "Manager.h"

namespace tezcat::Tiny::Core
{
	class ShaderPackage;
	class Shader;

	class TINY_API ShaderCreator
	{
	public:
		virtual ShaderPackage* create(const std::string& filePath) = 0;
	};

	class TINY_API ShaderManager : public Manager<ShaderCreator>
	{
	public:
		ShaderManager();
		~ShaderManager();

	public:
		int giveUID();

		void addShader(Shader* shader);
		void addShaderPackage(ShaderPackage* shaderPackage);
		ShaderPackage* findPackage(const std::string& name);
		ShaderPackage* create(const std::string& filePath);

	public:
		void loadIncludeFiles(const std::string& path);
		std::string getIncludeContent(const std::string& name);


	private:
		std::unordered_map<std::string, ShaderPackage*> mShaderPackageDict;
		std::vector<ShaderPackage*> mShaderPackageAry;
		std::vector<Shader*> mAllShaderList;

		std::unordered_map<std::string, std::string> mIncludeFileDict;
	public:
		void clearIncludeFiles();
	};

	using ShaderMgr = SG<ShaderManager>;
}

