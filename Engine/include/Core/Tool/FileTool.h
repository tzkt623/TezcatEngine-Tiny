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
#include "Core/Head/CppHead.h"
#include "Core/Head/ConfigHead.h"

namespace tezcat::Tiny
{
	enum FileType : uint32_t
	{
		FT_None = 0,
		FT_Text_Begin,
		FT_Text,
		FT_Tysl,
		FT_Tyin,
		FT_Text_End,

		FT_Img_Begin,
		FT_Jpg,
		FT_Png,
		FT_Hdr,
		FT_Img_End,

		FT_Model_Begin,
		FT_Fbx,
		FT_Obj,
		FT_PMX,
		FT_Model_End,

		FT_Unknown
	};

	struct FileInfo
	{
		FileType type;
		std::string path;
		std::string ext;
		std::string name;

		FileInfo()
			: type(FileType::FT_Unknown)
		{}

		FileInfo(FileInfo&& other) noexcept
			: type(other.type)
			, path(std::move(other.path))
			, ext(std::move(other.ext))
			, name(std::move(other.name))
		{

		}
	};

	typedef std::unordered_map<std::string, FileInfo> FileInfoMap;

	class TINY_API FileTool
	{
	public:
		static void init(const std::string& resourceFolder);
		static const std::string& getRootDir() { return EngineDir; }
		static const std::string& getRootResDir() { return EngineResDir; }

		static const std::string& getRootRelativeDir() { return EngineRelativeDir; }
		static const std::string& getRootRelativeResDir() { return EngineRelativeResDir; }

		static void loadFiles(const std::string& path, FileInfoMap& outFiles);

		static std::string loadText(const std::string& path);
		static void saveFile(const std::string& path, std::string& data);

		static void findAllFiles(const std::string& path, FileInfoMap& outFiles);

		static FileType getFileType(const std::string& ext);
	private:
		static std::string EngineDir;
		static std::string EngineResDir;

		static std::string EngineRelativeDir;
		static std::string EngineRelativeResDir;

		static std::unordered_map<std::string, FileType> sFileTypeUMap;

		static std::filesystem::path EnginePath;
		static void parseFileType(FileInfo &file_info);
	};
}

