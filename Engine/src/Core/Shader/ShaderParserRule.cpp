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

#include "Core/Shader/ShaderParserRule.h"

namespace tezcat::Tiny
{
	std::regex ShaderParserRule::GetComment(R"(/\*[\s\S]*\*/|//.*)");
	std::regex ShaderParserRule::GetAllAttribute(R"(@\[\w+\(.*\)\]\s*)");
	std::regex ShaderParserRule::GetOneAttribute(R"(@\[(\w+)\((.*?)\)\]\s*)");
	std::regex ShaderParserRule::GetPropertyInfo(R"((\w+)\((.*)\))");

	std::regex ShaderParserRule::GetTabAndEnter(R"(\t|\r|\n)");
	std::regex ShaderParserRule::GetBlank(R"(\s)");
	std::regex ShaderParserRule::SpliterSemicolon(R"(;)");
	std::regex ShaderParserRule::GetValueInfo(R"((\w+)\s(\w+)\s=\s([\s\S]+))");

	std::sregex_token_iterator ShaderParserRule::EndTokenIterator;
	std::sregex_iterator ShaderParserRule::EndIterator;

	std::regex ShaderParserRule::Tiny::GetNameInHead(R"(#TINY_HEAD_BEGIN\s*\{[\s\S]*str\s+Name\s+=\s+(.+);[\s\S]*\}\s*#TINY_HEAD_END)");
	std::regex ShaderParserRule::Tiny::GetHead(R"(#TINY_HEAD_BEGIN\s*\{\s*([\s\S]*)\}\s*#TINY_HEAD_END)");
	std::regex ShaderParserRule::Tiny::GetPass(R"(#TINY_PASS_BEGIN\s*\{\s*([\s\S]*?)\}\s*#TINY_PASS_END)");
	std::regex ShaderParserRule::Tiny::GetConfig(R"(#TINY_CFG_BEGIN\s*\{\s*([\s\S]*)\}\s*#TINY_CFG_END)");
	std::regex ShaderParserRule::Tiny::GetVS(R"(#TINY_VS_BEGIN\s*\{\s*([\s\S]*)\}\s*#TINY_VS_END)");
	std::regex ShaderParserRule::Tiny::GetFS(R"(#TINY_FS_BEGIN\s*\{\s*([\s\S]*)\}\s*#TINY_FS_END)");
	std::regex ShaderParserRule::Tiny::GetGS(R"(#TINY_GS_BEGIN\s*\{\s*([\s\S]*)\}\s*#TINY_GS_END)");
	std::regex ShaderParserRule::Tiny::GetIncludeFile(R"(#include\s+\"(\S+)\"\s*)");

	std::regex ShaderParserRule::UniformBuffer::GetOneBuffer(R"(@\[\s*Binding\((\d+)\)\s*\]\s*layout\(std\d*\)\s+uniform\s+(\w+)\s*\{([\s\S]*?)\};)");
	std::regex ShaderParserRule::UniformBuffer::GetProperty(R"(@\[\s*Binding\((\d+)\)\s*\]\s)");
	std::regex ShaderParserRule::UniformBuffer::SplitValue(R"((\w)+\s+(\w+)[\[*\d\]]*;)");


	//没有+后面的?就会进入贪婪模式匹配到所有struct甚至一堆无用的数据
	std::regex ShaderParserRule::Struct::GetOneStruct(R"(struct\s+(\w+)\s*\{([\w\W]+?)\s*\};)");
	//解析带属性的变量
	std::regex ShaderParserRule::Struct::GetMemberValue(R"((?:(@\[\w+\(.*\)\])\s*)*(\w+)\s+(\w+)\[?(\d*)\]?[\s\S]*?;)");


	//std::regex ShaderParserRule::Uniform::GetOneUniform(R"((?:(@\[\w+\(.*\)\])\s*)*uniform\s+(\w+)\s+(\w+)\[?(\d*)\]?[\s\S]*?;)");
	std::regex ShaderParserRule::Uniform::GetOneUniform(R"((@\[\w+\(.*\)\]\s*)*uniform\s+(\w+)\s+(\w+)\[?(\d*)\]?[\s\S]*?;)");
	std::regex ShaderParserRule::Uniform::GetAttributeValue(R"((\w+)\((.*)\))");

	std::regex ShaderParserRule::Attribute::GetRangeValue(R"((\d+),(\d+))");
	std::regex ShaderParserRule::Attribute::GetOneAttributeData(R"((\w+)\((.*?)\))");
}
