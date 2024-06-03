#include "ShaderManager.h"
#include "../Shader/Shader.h"
#include "../Shader/ShaderPackage.h"
#include "../Tool/FileTool.h"
#include "../Renderer/BaseGraphics.h"

namespace tezcat::Tiny
{
	std::unordered_map<std::string_view, Shader*> ShaderManager::mShaderUMap;

	void ShaderManager::loadShaderFiles(const std::string& path)
	{
		FileInfoMap out_files;
		FileTool::findAllFiles(path, out_files);

		for (auto& pair : out_files)
		{
			createShader(pair.second.path);
		}
	}

	void ShaderManager::rebuildShaders()
	{
		for (auto& pair : mShaderUMap)
		{
			Graphics::getInstance()->cmdDeleteShader(pair.second->getProgramID());
			Graphics::getInstance()->cmdCreateShader(pair.second);
		}
	}

	void ShaderManager::rebuild(const std::string& shaderName, std::string& data)
	{
		auto it = mShaderUMap.find(shaderName);
		if (it != mShaderUMap.end())
		{
			Graphics::getInstance()->cmdDeleteShader(it->second->getProgramID());
			Graphics::getInstance()->cmdCreateShader(it->second, data);
		}
	}

	Shader* ShaderManager::find(const std::string& name)
	{
		auto result = mShaderUMap.find(name);
		if (result != mShaderUMap.end())
		{
			return (*result).second;
		}

		return mShaderUMap["Unlit/Color"];
	}

	Shader* ShaderManager::createShader(const std::string& filePath)
	{
		auto shader = Shader::create(filePath);
		shader->generate();
		return shader;
	}

	void ShaderManager::registerShader(Shader* shader)
	{
		auto result = mShaderUMap.try_emplace(shader->getName(), shader);
		if (result.second)
		{
			shader->saveObject();
		}
	}


}

