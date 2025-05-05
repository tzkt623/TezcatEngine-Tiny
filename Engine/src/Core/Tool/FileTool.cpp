#include "Core/Tool/FileTool.h"

namespace tezcat::Tiny
{
	std::string FileTool::EngineDir;
	std::string FileTool::EngineResDir;

	std::string FileTool::EngineRelativeDir;
	std::string FileTool::EngineRelativeResDir;

	std::filesystem::path FileTool::EnginePath;

	std::unordered_map<std::string, FileType> FileInfo::sFileTypeUMap =
	{
		{ ".text",		FileType::FT_Text },
		{ ".tysl",		FileType::FT_Tysl },
		{ ".tyin",		FileType::FT_Tyin },
		{ ".tymeta",	FileType::FT_TyMeta },
		{ ".jpg",		FileType::FT_Jpg },
		{ ".png",		FileType::FT_Png },
		{ ".hdr",		FileType::FT_Hdr },
		{ ".fbx",		FileType::FT_Fbx },
		{ ".pmx",		FileType::FT_PMX },
		{ ".obj",		FileType::FT_Obj }
	};

	void FileInfo::parseFileType(FileInfo& fileInfo)
	{
		auto it = sFileTypeUMap.find(fileInfo.ext);
		if (it != sFileTypeUMap.end())
		{
			fileInfo.type = it->second;
		}
	}

	FileType FileInfo::getFileType(const std::string& ext)
	{
		auto it = sFileTypeUMap.find(ext);
		if (it != sFileTypeUMap.end())
		{
			return it->second;
		}

		return FileType::FT_Unknown;
	}

	void FileTool::init()
	{
		//std::regex rg(R"(\\+)");
		//
		//EnginePath = std::filesystem::current_path();
		//EngineDir = EnginePath.string();
		//EngineDir = std::regex_replace(EngineDir, rg, "/");
		//
		//if (!std::filesystem::exists(EngineDir))
		//{
		//	throw std::logic_error("fatal : Engine RootPath not Exists!!!");
		//}
		//
		//if (!std::filesystem::exists(EngineDir + "/" + resourceFolder))
		//{
		//	throw std::logic_error("fatal : .exe and ResourceFolder[" + resourceFolder + "] must be in the same directory");
		//}
		//
		//EngineResDir = EngineDir + "/" + resourceFolder;
		//EngineRelativeDir = "./";
		//EngineRelativeResDir = "./" + resourceFolder;
	}

	void FileTool::loadFiles(const file_path& path, FileInfoMap& outFiles)
	{
		//auto dir = EngineRelativeResDir + path;
		findAllFiles(path, outFiles);
	}

	void FileTool::findAllFiles(const file_path& path, FileInfoMap& outFiles)
	{
		std::filesystem::path temp(path);
		std::filesystem::directory_entry entry(temp);
		if (entry.is_directory())
		{
			std::filesystem::directory_iterator list(temp);
			//必须是一个目录
			for (auto& it : list)
			{
				if (it.is_directory())
				{
					//auto child_path = path + "/" + it.path().filename().string();
					findAllFiles(it.path(), outFiles);
				}
				else
				{
					FileInfo file_info(it.path());
					outFiles.emplace(file_info.name, std::move(file_info));
				}
			}
		}
		else
		{
			throw std::logic_error("load path must a dir");
		}
	}

	std::string FileTool::loadText(const file_path& path)
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

	void FileTool::saveFile(const file_path& path, std::string& data)
	{
		std::fstream io(path, std::ios::out | std::ios::binary);
		if (io.is_open())
		{
			io << data;
		}
		io.close();
	}
}
