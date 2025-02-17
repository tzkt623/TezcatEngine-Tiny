﻿#pragma once


/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

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
#include "ShaderConfig.h"
#include "ShaderParam.h"

namespace tezcat::Tiny
{
	class Shader;
	class ShaderParser
	{
	private:


	public:
		void parseHeader(std::string& content);
		void parseShaders(std::string& content, std::string& rootPath);
		void updateShaderConfig(Shader* shader);

	private:
		void parseShader(std::string& content, std::string& rootPath, const char* regex, std::string& outContent);

	public:
		static bool splitValue(std::string& content, std::unordered_map<std::string, Any>& map);
		static bool splitConfig(const std::string& content, std::string& config, std::string& suffix, const char* regex);
		static void splitPasses(std::string& content, std::vector<std::string>& passArray);

	public:
		std::unordered_map<std::string, Any> mConfigUMap;
		std::vector<std::string> mPassData;
		std::unordered_map<std::string, Any> mHeadUMap;
		UniformID::USet mUniformSet;
		std::unordered_set<std::string> mUserSet;

		std::unordered_map<std::string_view, std::shared_ptr<ArgMetaData>> mTinyUMap;
		std::unordered_map<std::string_view, std::shared_ptr<ArgMetaData>> mUserUMap;
		std::unordered_map<std::string_view, std::shared_ptr<ArgMetaData>> mStructUMap;

		std::string mVertexShader;
		std::string mFragShader;
	};
}
