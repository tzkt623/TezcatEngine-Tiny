#include "Resource.h"

namespace tezcat::Tiny
{
	std::unordered_set<TinyObject*> Resource::mDataUSet;
	std::string Resource::sEngineDir;
	std::string Resource::sResDir;

	std::string Resource::sRelativeEngineDir;
	std::string Resource::sRelativeResDir;




	void Resource::init(const std::string& folderPath)
	{
		std::regex rg(R"(\\+)");

		sEngineDir = std::filesystem::current_path().string();
		sEngineDir = std::regex_replace(sEngineDir, rg, "/");

		if (!std::filesystem::exists(sEngineDir))
		{
			throw std::logic_error("fatal : Engine Path not Exists!!!");
		}

		if (!std::filesystem::exists(sEngineDir + "/" + folderPath))
		{
			throw std::logic_error("fatal : .exe and ResourceFolder[" + folderPath + "] must be in the same directory");
		}

		sResDir = sEngineDir + "/" + folderPath;

		sRelativeEngineDir = "./";
		sRelativeResDir = "./" + folderPath;
	}
}


