#pragma once
#include "../Head/CppHead.h"
#include "../Tool/Tool.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny
{
	class Shader;
	class TINY_API ShaderManager
	{
		ShaderManager() = delete;
		~ShaderManager() = delete;

	public:
		static Shader* find(const std::string& name);
		static Shader* createShader(const std::string& filePath);
		static std::unordered_map<std::string_view, Shader*>& getAllShaders() { return mShaderUMap; }
		static void registerShader(Shader* shader);

	public:
		static void loadShaderFiles(const std::string& path);

		static void rebuildShaders();
		static void rebuild(const std::string& shaderName, std::string& data);

	private:
		static std::unordered_map<std::string_view, Shader*> mShaderUMap;
	public:
	};
}

