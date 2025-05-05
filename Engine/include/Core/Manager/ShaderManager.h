#pragma once

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
		static const std::vector<Shader*>& getAllowUseShaders();
		static void registerShader(Shader* shader);

	public:
		static void loadShaderFiles(const std::string& path);

		static void rebuildShaders();
		static void rebuild(const std::string& shaderName, std::string& data);

	private:
		static std::unordered_map<std::string_view, Shader*> mShaderUMap;

		static std::unordered_map<std::string_view, Shader*> mCanUseUMap;
		static std::vector<Shader*> mCanUseArray;
		static bool mIsDirty;
	};
}

