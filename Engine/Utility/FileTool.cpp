#include "FileTool.h"
#include <iostream>
#include <filesystem>

namespace tezcat::Tiny::Utility
{
	std::string FileTool::EngineDir;
	std::string FileTool::EngineResDir;

	std::string FileTool::EngineRelativeDir;
	std::string FileTool::EngineRelativeResDir;

	std::filesystem::path FileTool::EnginePath;

	void FileTool::init(const std::string& resourceFolder)
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

	void FileTool::loadFiles(const std::string& path, std::unordered_map<std::string, std::string>& outFiles)
	{
		auto dir = EngineRelativeResDir + path;
		findAllFiles(dir, outFiles);
	}

	void FileTool::findAllFiles(const std::string& path, std::unordered_map<std::string, std::string>& outFiles)
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
					auto file_name = it.path().filename().string();
					outFiles.emplace(file_name.substr(0, file_name.find_first_of(".")), path + "/" + it.path().filename().string());
				}
			}
		}
		else
		{
			throw std::logic_error("load path must a dir");
		}
	}
}
