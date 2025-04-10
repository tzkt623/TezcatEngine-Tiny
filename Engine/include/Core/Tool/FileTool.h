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
		FT_TyMeta,
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
		file_path path;
		std::string ext;
		std::string name;

		FileInfo(const file_path& path)
		{
			this->setPath(path);
		}

		FileInfo(FileInfo&& other) noexcept
			: type(other.type)
			, path(std::move(other.path))
			, ext(std::move(other.ext))
			, name(std::move(other.name))
		{

		}

		void setPath(const file_path& path)
		{
			this->path = path;
			this->ext = path.extension().string();
			this->name = path.filename().string();
			parseFileType(*this);
		}
		static FileType getFileType(const std::string& ext);

	private:
		static void parseFileType(FileInfo& file_info);
		static std::unordered_map<std::string, FileType> sFileTypeUMap;
	};

	typedef std::unordered_map<std::string, FileInfo> FileInfoMap;

	class TINY_API FileTool
	{
	public:
		static void init();
		static const std::string& getRootDir() { return EngineDir; }
		static const std::string& getRootResDir() { return EngineResDir; }

		static const std::string& getRootRelativeDir() { return EngineRelativeDir; }
		static const std::string& getRootRelativeResDir() { return EngineRelativeResDir; }

		static void loadFiles(const file_path& path, FileInfoMap& outFiles);

		static std::string loadText(const std::string& path);
		static void saveFile(const std::string& path, std::string& data);

		static void findAllFiles(const file_path& path, FileInfoMap& outFiles);

	private:
		static std::string EngineDir;
		static std::string EngineResDir;

		static std::string EngineRelativeDir;
		static std::string EngineRelativeResDir;

		static std::filesystem::path EnginePath;
	};
}

