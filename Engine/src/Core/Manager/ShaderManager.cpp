/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Core/Manager/ShaderManager.h"

#include "Core/Shader/Shader.h"
#include "Core/Shader/ShaderPackage.h"

#include "Core/Tool/FileTool.h"

#include "Core/Renderer/RenderCommand.h"

#include "Core/Debug/Debug.h"

namespace tezcat::Tiny
{
	std::unordered_map<std::string_view, Shader*> ShaderManager::mShaderUMap;
	std::unordered_map<std::string_view, Shader*> ShaderManager::mCanUseUMap;
	std::vector<Shader*> ShaderManager::mCanUseArray;
	bool ShaderManager::mIsDirty = false;

	void ShaderManager::loadShaderFiles(const std::string& path)
	{
		FileInfoMap out_files;
		FileTool::findAllFiles(path, out_files);

		for (auto& pair : out_files)
		{
			createShader(pair.second.path.string());
		}
	}

	void ShaderManager::rebuildShaders()
	{
		for (auto& pair : mShaderUMap)
		{
			RenderCommandHelper::addCommand(new RenderCMD_RebuildShader(pair.second));
		}
	}

	void ShaderManager::rebuild(const std::string& shaderName, std::string& data)
	{
		auto it = mShaderUMap.find(shaderName);
		if (it != mShaderUMap.end())
		{
			it->second->mContent = std::make_unique<std::string>(std::move(data));
			RenderCommandHelper::addCommand<RenderCMD_RebuildShader>(it->second);
		}
	}

	Shader* ShaderManager::find(const std::string& name)
	{
		auto result = mShaderUMap.find(name);
		if (result != mShaderUMap.end())
		{
			return result->second;
		}

		TINY_LOG_ERROR(std::format("Shader Not Find {}", name));
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
		auto& name = shader->getName();
		auto result = mShaderUMap.try_emplace(name, shader);
		if (result.second)
		{
			shader->saveObject();

			if (!name.starts_with("Hide"))
			{
				mIsDirty = true;
				mCanUseUMap.emplace(shader->getName(), shader);
				mCanUseArray.push_back(shader);
			}
		}
	}

	const std::vector<Shader*>& ShaderManager::getAllowUseShaders()
	{
		if (mIsDirty)
		{
			mIsDirty = false;
			std::ranges::sort(mCanUseArray, [](Shader* a, Shader* b)
				{
					auto result = a->getName().compare(b->getName());

					return result;
				});
		}

		return mCanUseArray;
	}

}

