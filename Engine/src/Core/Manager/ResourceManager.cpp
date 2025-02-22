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

#include "Core/Manager/ResourceManager.h"

namespace tezcat::Tiny
{
	std::unordered_set<TinyObject*> ResourceManager::mDataUSet;
	std::string ResourceManager::sEngineDir;
	std::string ResourceManager::sResDir;

	std::string ResourceManager::sRelativeEngineDir;
	std::string ResourceManager::sRelativeResDir;

	std::unordered_map<std::string, Model*> ResourceManager::mModelMap;



	void ResourceManager::init(const std::string& folderPath)
	{
		std::regex rg(R"(\\+)");

		sEngineDir = std::filesystem::current_path().string();
		sEngineDir = std::regex_replace(sEngineDir, rg, "/");

		if (!std::filesystem::exists(sEngineDir))
		{
			throw std::logic_error("fatal : Engine Path not Exists!!!");
		}

		if (!std::filesystem::exists(std::format("{}/{}", sEngineDir, folderPath)))
		{
			throw std::logic_error("fatal : .exe and ResourceFolder[" + folderPath + "] must be in the same directory");
		}

		sResDir = std::format("{}/{}", sEngineDir, folderPath);

		sRelativeEngineDir = "./";
		sRelativeResDir = std::format("./{}", folderPath);
	}


}


