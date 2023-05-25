#pragma once

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

