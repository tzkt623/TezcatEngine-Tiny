#pragma once
#include "../Head/CppHead.h"
#include "../Tool/Tool.h"
#include "../Head/ConfigHead.h"
#include "Manager.h"

namespace tezcat::Tiny
{
	class Shader;
	class TINY_API ShaderManager
	{
	public:
		ShaderManager();
		~ShaderManager();

	public:
		Shader* find(const std::string& name);
		Shader* createShader(const std::string& filePath);
		std::unordered_map<std::string, Shader*>& getAllShaders() { return mShaderUMap; }
		void registerShader(Shader* shader);

	public:
		void loadShaderFiles(const std::string& path);

		void rebuildShaders();
		void rebuild(std::filesystem::path& path);
		void rebuild(const std::string& shaderName, std::string& data);

	private:
		std::unordered_map<std::string, Shader*> mShaderUMap;
	public:
	};

	using ShaderMgr = SG<ShaderManager>;
}

