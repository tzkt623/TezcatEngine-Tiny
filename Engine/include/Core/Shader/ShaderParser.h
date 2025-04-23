#pragma once


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
		//移除约束条件
		std::regex regex_remove_constraint;
		//移除注释
		std::regex regex_comment;
		//结束位置
		std::sregex_iterator end;

	public:
		ShaderParser();

		void parse(const std::string& path);
		void parse(std::string& content, const std::string& path);
		void updateShaderConfig(Shader* shader);

	private:
		void parseHeader(std::string& content);
		void parseShaders(std::string& content, std::string& rootPath);

	private:
		bool parseShader(std::string& content, std::string& rootPath, const char* regex, std::string& outContent);
		void removeComment(std::string& content);
		void splitInclude(std::string& content, const std::string& rootPath);
		void splitUniformBuffer(std::string& content);
		void splitStruct(std::string& content);
		void writeShaderHead(std::string& content);

	public:
		static bool splitValue(std::string& content, std::unordered_map<std::string, Any>& map);
		static bool splitConfig(const std::string& content, std::string& config, std::string& suffix, const char* regex);
		static void splitPasses(std::string& content, std::vector<std::string>& passArray);
		static std::string getName(const std::string& textToSave);

	public:
		std::unordered_map<std::string, Any> mConfigUMap;
		std::vector<std::string> mPassData;
		std::unordered_map<std::string, Any> mHeadUMap;
		UniformID::USet mUniformSet;
		std::unordered_set<std::string> mUserSet;

		std::unordered_map<std::string_view, std::shared_ptr<ShaderUniformMember>> mTinyUMap;
		std::unordered_map<std::string_view, std::shared_ptr<ShaderUniformMember>> mUserUMap;
		std::unordered_map<std::string_view, std::shared_ptr<ShaderUniformMember>> mStructUMap;

		std::string mVertexShader;
		std::string mFragShader;
		std::string mGeometryShader;

		std::unordered_map<std::string, int32_t> mUBOMap;
	};
}
