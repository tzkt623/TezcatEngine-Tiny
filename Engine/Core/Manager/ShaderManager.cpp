#include "ShaderManager.h"
#include "../Shader/Shader.h"
#include "../Shader/ShaderPackage.h"
#include "../Tool/FileTool.h"

namespace tezcat::Tiny
{
	ShaderManager::ShaderManager()
	{
		SG<ShaderManager>::attach(this);
	}

	ShaderManager::~ShaderManager()
	{

	}

	int ShaderManager::giveUID()
	{
		int uid = static_cast<int>(mShaderPackageAry.size());
		mShaderPackageAry.push_back(nullptr);
		return uid;
	}

	void ShaderManager::addShader(Shader* shader)
	{
		while (mAllShaderList.size() <= shader->getUID())
		{
			mAllShaderList.push_back(nullptr);
		}
		mAllShaderList[shader->getUID()] = shader;
	}

	void ShaderManager::addShaderPackage(ShaderPackage* shaderPackage)
	{
		if (mShaderPackageAry[shaderPackage->getUID()] == nullptr)
		{
			mShaderPackageAry[shaderPackage->getUID()] = shaderPackage;
			mShaderPackageDict.emplace(shaderPackage->getName(), shaderPackage);
		}
		else
		{
			throw std::invalid_argument(StringTool::stringFormat("ShaderManager: This ShaderPackage[%s] has stored[%d]",
				shaderPackage->getName().c_str(), shaderPackage->getUID()));
		}
	}

	ShaderPackage* ShaderManager::findPackage(const std::string& name)
	{
		auto it = mShaderPackageDict.find(name);
		if (it != mShaderPackageDict.end())
		{
			return it->second;
		}

		return mShaderPackageDict["Unlit/Color"];
	}

	void ShaderManager::loadIncludeFiles(const std::string& path)
	{
		//------------------------------------------------------------
		//
		//	删除注释
		//	包括//型 和 /**/型
		//
		std::regex regex_comment(R"(/\*[\s\S]*\*/|//.*)");


		FileInfoMap out_files;
		FileTool::findAllFiles(path, out_files);

		struct DataInfo
		{
			std::string data;
			//过滤掉重复头文件
			std::unordered_set<std::string> includeChecker;

			DataInfo(const std::string& info) : data(info) { }
		};

		std::unordered_map<std::string_view, DataInfo> temp_datas;
		//预加载
		for (auto& pair : out_files)
		{
			auto& info = pair.second;
			std::string data = FileTool::loadText(info.path);
			data = std::regex_replace(data, regex_comment, "");
			temp_datas.emplace(pair.first, DataInfo(data));
			mIncludeFileDict.emplace(info.name, data);
		}

		//------------------------------------------------------------
		//
		//	分析Include
		//
		//
		std::regex regex_include(R"(#include\s*\"(\S+)\"\s*)");
		std::sregex_iterator end;
		std::vector<std::string> include_name_ary;

		while (!temp_datas.empty())
		{
			for (auto it = temp_datas.begin(); it != temp_datas.end();)
			{
				std::string temp = it->second.data;
				for (auto include_it = std::sregex_iterator(temp.begin(), temp.end(), regex_include); include_it != end; include_it++)
				{
					std::string include_name = (*include_it)[1];

					//过滤掉已经加入的头文件
					if (it->second.includeChecker.contains(include_name))
					{
						continue;
					}

					include_name_ary.emplace_back(include_name);
					it->second.includeChecker.emplace(include_name);
				}

				if (!include_name_ary.empty())
				{
					//组合shader
					temp = std::regex_replace(temp, regex_include, "");

					std::string inlcude_data;
					for (auto& name : include_name_ary)
					{
						inlcude_data += mIncludeFileDict[name];
					}

					temp = inlcude_data + temp;
					it->second = temp;
					include_name_ary.clear();
					it++;
				}
				else
				{
					mIncludeFileDict[it->first.data()] = std::move(it->second.data);
					it = temp_datas.erase(it);
				}
			}
		}
	}

	ShaderPackage* ShaderManager::create(const std::string& filePath)
	{
		auto pack = mCreator->create(filePath);
		this->addShaderPackage(pack);
		return pack;
	}

	void ShaderManager::clearIncludeFiles()
	{
		//mIncludeFileDict.clear();
	}

	std::string ShaderManager::getIncludeContent(const std::string& s)
	{
		return mIncludeFileDict[s];
	}

	void ShaderManager::rebuildShaders()
	{
		for (auto p : mShaderPackageAry)
		{
			mCreator->rebuild(p);
		}
	}

	void ShaderManager::rebuild(ShaderPackage* package)
	{
		mCreator->rebuild(package);
	}
}

