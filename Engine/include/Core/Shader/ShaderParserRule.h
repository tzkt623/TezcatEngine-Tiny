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

#include <regex>

namespace tezcat::Tiny
{
	struct ShaderParserRule
	{
		static std::regex GetComment;
		static std::regex GetAllAttribute;
		static std::regex GetOneAttribute;
		static std::regex GetPropertyInfo;

		static std::regex GetTabAndEnter;
		static std::regex GetBlank;

		static std::regex SpliterSemicolon;
		static std::regex GetValueInfo;

		static std::sregex_token_iterator EndTokenIterator;
		static std::sregex_iterator EndIterator;

		struct Tiny
		{
			static std::regex GetNameInHead;
			static std::regex GetHead;
			static std::regex GetPass;
			static std::regex GetConfig;
			static std::regex GetVS;
			static std::regex GetFS;
			static std::regex GetGS;
			static std::regex GetIncludeFile;
		};

		struct Struct
		{
			static std::regex GetOneStruct;
			static std::regex GetMemberValue;
		};

		struct UniformBuffer
		{
			static std::regex GetOneBuffer;
			static std::regex GetProperty;
			static std::regex SplitValue;
		};

		struct Uniform
		{
			static std::regex GetOneUniform;
			static std::regex GetAttributeValue;
		};

		struct Attribute
		{
			static std::regex GetRangeValue;
			static std::regex GetOneAttributeData;
		};
	};
}

