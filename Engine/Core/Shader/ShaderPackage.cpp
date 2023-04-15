#include "ShaderPackage.h"
#include "Shader.h"

#include "Utility/Utility.h"
#include "../Manager/ShaderManager.h"

namespace tezcat::Tiny::Core
{
	ShaderPackage::ShaderPackage()
		: mUID(-1)
	{

	}


	ShaderPackage::~ShaderPackage()
	{

	}

	void ShaderPackage::setConfig(std::unordered_map<std::string, Any>& data)
	{
		mName = data["Name"].cast<std::string>();
	}

	void ShaderPackage::addShader(Shader* shader)
	{
		mShaders.push_back(shader);
	}

	void ShaderPackage::apply()
	{
		mUID = SG<ShaderManager>::getInstance()->giveUID();

		if (mShaders.size() > 1)
		{
			std::sort(mShaders.begin(), mShaders.end(),
				[](Shader* a, Shader* b)
				{
					return a->getOrderID() < b->getOrderID();
				});
		}

		SG<ShaderManager>::getInstance()->addShaderPackage(this);
	}


	std::unordered_map<std::string, std::string> ShaderCreator::sIncludeFileDict;
	void ShaderCreator::loadIncludeFiles(const std::string& path)
	{
		//------------------------------------------------------------
		//
		//	删除注释
		//	包括//型 和 /**/型
		//
		std::regex regex_comment(R"(/\*[\s\S]*\*/|//.*)");


		std::unordered_map<std::string, std::string> out_files;
		FileTool::findAllFiles(path, out_files);

		struct DataInfo
		{
			std::string data;
			//过滤掉重复头文件
			std::unordered_set<std::string> includeChecker;

			DataInfo(const std::string& info) : data(info) { }
		};

		std::unordered_map<std::string, DataInfo> temp_datas;
		//预加载
		for (auto& pair : out_files)
		{
			std::string data = FileTool::loadText(pair.second);
			data = std::regex_replace(data, regex_comment, "");
			temp_datas.emplace(pair.first, DataInfo(data));
			sIncludeFileDict.emplace(pair.first, data);
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
						inlcude_data += sIncludeFileDict[name];
					}

					temp = inlcude_data + temp;
					it->second = temp;
					include_name_ary.clear();
					it++;
				}
				else
				{
					sIncludeFileDict[it->first] = it->second.data;
					it = temp_datas.erase(it);
				}
			}
		}
	}

	std::string ShaderCreator::getIncludeContent(std::string& name)
	{
		return sIncludeFileDict[name];
	}

	void ShaderCreator::clearIncludeFiles()
	{
		sIncludeFileDict.clear();
	}
}
