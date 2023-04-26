#include "FileTool.h"
#include <iostream>
#include <filesystem>

namespace tezcat::Tiny
{
	std::string FileTool::EngineDir;
	std::string FileTool::EngineResDir;

	std::string FileTool::EngineRelativeDir;
	std::string FileTool::EngineRelativeResDir;

	std::filesystem::path FileTool::EnginePath;

	std::unordered_map<std::string, FileType> FileTool::sFileTypeUMap =
	{
		{ "text", FileType::FT_Text },
		{ "jpg", FileType::FT_Jpg },
		{ "png", FileType::FT_Png },
		{ "hdr", FileType::FT_Hdr }
	};

	void FileTool::init(const std::string & resourceFolder)
	{
		std::regex rg(R"(\\+)");

		EnginePath = std::filesystem::current_path();
		EngineDir = EnginePath.string();
		EngineDir = std::regex_replace(EngineDir, rg, "/");

		if (!std::filesystem::exists(EngineDir))
		{
			throw std::logic_error("fatal : Engine RootPath not Exists!!!");
		}

		if (!std::filesystem::exists(EngineDir + "/" + resourceFolder))
		{
			throw std::logic_error("fatal : .exe and ResourceFolder[" + resourceFolder + "] must be in the same directory");
		}

		EngineResDir = EngineDir + "/" + resourceFolder;
		EngineRelativeDir = "./";
		EngineRelativeResDir = "./" + resourceFolder;
	}

	void FileTool::loadFiles(const std::string& path, FileInfoMap& outFiles)
	{
		auto dir = EngineRelativeResDir + path;
		findAllFiles(dir, outFiles);
	}

	void FileTool::findAllFiles(const std::string& path, FileInfoMap& outFiles)
	{
		std::filesystem::path temp(path);
		std::filesystem::directory_entry entry(temp);
		if (entry.status().type() == std::filesystem::file_type::directory)
		{
			std::filesystem::directory_iterator list(temp);
			//必须是一个目录
			for (auto& it : list)
			{
				if (it.is_directory())
				{
					auto child_path = path + "/" + it.path().filename().string();
					findAllFiles(child_path, outFiles);
				}
				else
				{
					auto name = it.path().filename().string();
					auto pos = name.find_last_of(".");
					FileInfo file_info;
					file_info.path = std::move(path + "/" + name);
					file_info.name = std::move(name.substr(0, pos));
					file_info.exp = std::move(name.substr(pos + 1, name.size() - pos));

					FileTool::parseFileType(file_info);
					outFiles.emplace(file_info.name, std::move(file_info));
				}
			}
		}
		else
		{
			throw std::logic_error("load path must a dir");
		}
	}

	std::string FileTool::loadText(const std::string& path)
	{
		std::string data;
		std::fstream io(path, std::ios::in | std::ios::binary);
		if (io.is_open())
		{
			std::stringstream buf;
			buf << io.rdbuf();
			data = buf.str();
		}
		io.close();
		return data;
	}

	void FileTool::parseFileType(FileInfo& fileInfo)
	{
		auto it = sFileTypeUMap.find(fileInfo.exp);
		if (it != sFileTypeUMap.end())
		{
			fileInfo.type = it->second;
		}
	}
}
