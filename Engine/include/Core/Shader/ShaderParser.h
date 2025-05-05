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

#include "../Head/TinyCpp.h"
//#include "ShaderConfig.h"
#include "ShaderParam.h"
#include "ShaderReflection.h"

namespace tezcat::Tiny
{
	class Shader;
	class ShaderParser
	{
	public:
		ShaderParser();

		void parse(const std::string& path);
		void parse(std::string& content, const std::string& path);
		void updateShaderConfig(Shader* shader);

	private:
		void parseHeader(std::string& content);
		void parseShaders(std::string& content, std::string& rootPath);

	private:
		bool parseShader(std::string& content, std::string& rootPath, const std::regex& regex, std::string& outContent);
		void removeComment(std::string& content);
		void splitInclude(std::string& content, const std::string& rootPath);
		void splitUniformBuffer(std::string& content);
		void splitStruct(std::string& content);
		void writeShaderHead(std::string& content);

		void parseAttribute(const std::string& attributeMatch, ShaderMetaDataMember* member);
		void parseUniformValue(std::string& content);
		void parseInclude(std::string& include_content, std::unordered_set<uint64_t>& check_includes, std::vector<file_path>& all_includes, const file_path& rootPath);
		void parseEditorAttribute(const std::ssub_match& attributeMatch, ShaderMetaDataMember* member);

	public:
		static bool splitValue(std::string& content, std::unordered_map<std::string, Any>& map);
		static bool splitConfig(const std::string& content, std::string& config, std::string& suffix, const std::regex& regex);
		static void splitPasses(std::string& content, std::vector<std::string>& passArray);
		static std::string getName(const std::string& textToSave);

	public:
		std::unordered_map<std::string, Any> mConfigUMap;
		std::vector<std::string> mPassData;
		std::unordered_map<std::string, Any> mHeadUMap;
		UniformID::USet mUniformSet;
		std::unordered_set<std::string> mUserSet;

		std::unordered_map<std::string_view, std::unique_ptr<ShaderMetaDataArgument>> mTinyUMap;
		std::unordered_map<std::string_view, std::unique_ptr<ShaderMetaDataArgument>> mUserUMap;
		std::unordered_map<std::string_view, std::unique_ptr<ShaderMetaDataStruct>> mStructUMap;
		std::unordered_map<std::string_view, std::unique_ptr<ShaderMetaDataLayoutStruct>> mUBOMap;

		std::vector<ShaderMetaDataArgument*> mGlobalUniform;

		std::string mVertexShader;
		std::string mFragShader;
		std::string mGeometryShader;

		//std::unordered_map<std::string, int32_t> mUBOMap;
	};
}
