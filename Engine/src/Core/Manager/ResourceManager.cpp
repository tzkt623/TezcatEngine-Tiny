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
#include "Core/Debug/Log.h"

namespace tezcat::Tiny
{
	std::unordered_set<TinyObject*> ResourceManager::mDataUSet;
	std::string ResourceManager::sEngineDir;
	std::string ResourceManager::sResDir;

	std::string ResourceManager::sRelativeEngineDir;
	std::string ResourceManager::sRelativeResDir;

	std::unordered_map<file_path, Model*> ResourceManager::mModelMap;
	std::unordered_map<file_path, Texture2D*> ResourceManager::mTexture2DUMap;
	std::unordered_map<file_path, Image*> ResourceManager::mImageMap;
	std::unordered_map<file_path, Image*> ResourceManager::mTinyImageMap;
	std::unordered_map<std::string, std::function<void(std::string&)>> ResourceManager::mMetaLoadMap;

	void ResourceManager::init()
	{
		sRelativeEngineDir = "./";

		mMetaLoadMap["RenderTexture"] = [](std::string& content)
			{
				//MetaFile
			};
	}

	void ResourceManager::loadMetaFile(const file_path& path)
	{
		std::regex split(R"((\w)+ +(\w)+)");
		std::ifstream file;
		file.open(path, std::ios::binary);
		if (!file.is_open())
		{
			return;
		}

		std::string line;
		std::getline(file, line);

		//获取元数据类型
		std::vector<std::string> type;
		auto split_string_view = line | std::ranges::views::split(' ');
		for (auto&& subrange : split_string_view)
		{
			// 通过子范围的迭代器构造字符串
			type.emplace_back(subrange.begin(), subrange.end());
		}

		auto it = mMetaLoadMap.find(type[1]);
		if (it != mMetaLoadMap.end())
		{
			std::string content;
			while (std::getline(file, line))
			{
				content.append(line);
			}

			it->second(content);
		}

		file.close();
	}


}
