#pragma once

#include "Core/Head/CppHead.h"
#include "Core/Head/ConfigHead.h"

namespace tezcat::Tiny::Utility
{
	class TINY_API FileTool
	{
	public:
		static void init(const std::string& resourceFolder);
		static const std::string& getRootDir() { return EngineDir; }
		static const std::string& getRootResDir() { return EngineResDir; }

		static const std::string& getRootRelativeDir() { return EngineRelativeDir; }
		static const std::string& getRootRelativeResDir() { return EngineRelativeResDir; }

		static void loadFiles(const std::string& path, std::unordered_map<std::string, std::string>& outFiles);

		static std::string loadText(const std::string& path);

		static void findAllFiles(const std::string& path, std::unordered_map<std::string, std::string>& outFiles);

	private:
		static std::string EngineDir;
		static std::string EngineResDir;

		static std::string EngineRelativeDir;
		static std::string EngineRelativeResDir;


		static std::filesystem::path EnginePath;
	};
}

