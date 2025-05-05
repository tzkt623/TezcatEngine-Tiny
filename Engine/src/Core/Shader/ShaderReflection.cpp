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

#include "Core/Shader/ShaderReflection.h"

namespace tezcat::Tiny
{

	ShaderMetaDataMember* ShaderMetaDataStruct::addMemeber(const std::string& name, const UniformType& uniformType)
	{
		auto memeber = std::make_shared<ShaderMetaDataMember>();
		memeber->name = name;
		memeber->valueType = uniformType;
		members.emplace_back(memeber);
		return memeber.get();
	}

	ShaderMetaDataStruct* ShaderMetaDataStruct::addStruct(const std::string& name)
	{
		auto memeber = std::make_shared<ShaderMetaDataStruct>();
		memeber->name = name;
		members.emplace_back(memeber);
		return memeber.get();
	}

}


