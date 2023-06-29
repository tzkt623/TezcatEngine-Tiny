#include "GLShaderBuilder.h"
#include "GLHead.h"

#include "Core/Shader/Shader.h"
#include "Core/Head/RenderConfig.h"
#include "Core/Manager/ShaderManager.h"
#include "Core/Shader/ShaderPackage.h"
#include "Core/Tool/Log.h"
#include "ThirdParty/Hash/city.h"


namespace tezcat::Tiny::GL
{
	GLShaderBuilder::GLShaderBuilder()
	{

	}

	GLShaderBuilder::~GLShaderBuilder()
	{

	}

	bool GLShaderBuilder::splitConfig(const std::string& content, std::string& config, std::string& suffix, const char* regex)
	{
		std::regex reg(regex);
		std::smatch result;
		if (std::regex_search(content, result, reg))
		{
			config = result[1];
			suffix = result.suffix();
			//			std::cout << result[1] << std::endl;
			return true;
		}

		return false;
	}

	bool GLShaderBuilder::splitValue(std::string& content, std::unordered_map<std::string, Any>& map)
	{
		std::regex regex_empty(R"(\t|\r|\n)");
		content = std::regex_replace(content, regex_empty, "");

		std::regex regex_spliter(";");
		std::sregex_token_iterator end;

		std::regex regex_value_pair(R"((\w+)\s(\w+)\s=\s([\s\S]+))");
		std::smatch value_pair_result;

		std::string type, name, value, temp;

		for (auto i = std::sregex_token_iterator(content.begin(), content.end(), regex_spliter, -1); i != end; i++)
		{
			temp.assign(std::move((*i).str()));
			if (std::regex_search(temp, value_pair_result, regex_value_pair))
			{
				type.assign(std::move(value_pair_result[1].str()));
				name.assign(std::move(value_pair_result[2].str()));
				value.assign(std::move(value_pair_result[3].str()));

				if (type == "str")
				{
					map.emplace(std::move(name), std::move(value));
				}
				else if (type == "int")
				{
					map.emplace(std::move(name), std::stoi(value));
				}
				else if (type == "float")
				{
					map.emplace(std::move(name), std::stof(value));
				}
				else if (type == "bool")
				{
					if (value == "true")
					{
						map.emplace(std::move(name), true);
					}
					else if (value == "false")
					{
						map.emplace(std::move(name), false);
					}
					else
					{
						Log_Error("GLShader: Shader Param [bool]`s string must [true] or [false]");
						//throw std::logic_error("GLShader: Shader Param [bool]`s string must [true] or [false]");
					}
				}
			}
		}

		return !map.empty();
	}

	GLShader* GLShaderBuilder::loadFromFile(const char* filePath)
	{
		GLShader* shader = nullptr;
		std::fstream io(filePath, std::ios::in | std::ios::binary);
		if (io.is_open())
		{
			std::string data;
			std::stringstream buf;
			buf << io.rdbuf();
			data = buf.str();

			//this->splitPackage(data);
		}
		io.close();
		return shader;
	}

	void GLShaderBuilder::splitPasses(std::string& content)
	{
		std::regex pattern(R"(#TINY_PASS_BEGIN\s*\{\s*([\s\S]*?)\}\s*#TINY_PASS_END)");
		std::sregex_iterator end;
		for (auto i = std::sregex_iterator(content.begin(), content.end(), pattern); i != end; i++)
		{
			//UniformID::USet set;

			std::string temp((*i)[1]);
			mPassData.push_back(std::move(temp));
		}
	}


	void GLShaderBuilder::updateShaderConfig(Shader* shader)
	{
		auto& map = mConfigUMap;
		shader->setVersion(map["Version"].cast<int>());
		shader->setName(map["Name"].cast<std::string>());

		//OrderID
		auto it = map.find("OrderID");
		if (it != map.end())
		{
			shader->setOrderID(it->second.cast<int>());
		}
		else
		{
			shader->setOrderID(2000);
		}

		//Queue
		it = map.find("Queue");
		if (it != map.end())
		{
			shader->setRenderQueue(ContextMap::QueueMap[it->second.cast<std::string>()]);
		}
		else
		{
			shader->setRenderQueue(Queue::None);
		}

		//LightMode
		it = map.find("LightMode");
		if (it != map.end())
		{
			shader->setLightMode(ContextMap::LightModeMap[it->second.cast<std::string>()]);
		}
		else
		{
			shader->setLightMode(LightMode::Unlit);
		}

		//DepthTest
		it = map.find("DepthTest");
		if (it != map.end())
		{
			shader->setDepthTest(ContextMap::DepthTestMap[it->second.cast<std::string>()]);
			//ZWrite
			it = map.find("ZWrite");
			if (it != map.end())
			{
				shader->setZWrite(it->second.cast<bool>());
			}
			else
			{
				shader->setZWrite(false);
			}
		}
		else
		{
			shader->setDepthTest(DepthTest::Off);
		}


		//Blend
		it = map.find("Blend");
		if (it != map.end() && it->second.cast<bool>())
		{
			shader->setBlend(true);
			shader->setBlendFunction(ContextMap::BlendMap[map["BlendSrc"].cast<std::string>()]
				, ContextMap::BlendMap[map["BlendTar"].cast<std::string>()]);
		}
		else
		{
			shader->setBlend(false);
		}

		//CullFace
		it = map.find("CullFace");
		if (it != map.end())
		{
			shader->setCullFace(ContextMap::CullFaceMap[it->second.cast<std::string>()]);
		}
	}

	void GLShaderBuilder::parseHeader(std::string& content)
	{
		std::regex regex(R"(#TINY_HEAD_BEGIN\s*\{\s*([\s\S]*)\}\s*#TINY_HEAD_END)");
		std::smatch result;
		if (std::regex_search(content, result, regex))
		{
			std::string temp = result[1];
			content = result.suffix();
			splitValue(temp, mHeadUMap);
		}
	}

	void GLShaderBuilder::parseShaders(std::string& content, std::string& rootPath)
	{
		std::string config, shader_content;
		if (splitConfig(content, config, shader_content, R"(#TINY_CFG_BEGIN\s*\{\s*([\s\S]*)\}\s*#TINY_CFG_END)"))
		{
			splitValue(config, mConfigUMap);
			this->parseShader(shader_content
				, rootPath
				, R"(#TINY_VS_BEGIN\s*\{\s*([\s\S]*)\}\s*#TINY_VS_END)"
				, mVertexShader);
			this->parseShader(shader_content
				, rootPath
				, R"(#TINY_FS_BEGIN\s*\{\s*([\s\S]*)\}\s*#TINY_FS_END)"
				, mFragShader);
		}
		else
		{
			throw std::logic_error("GLShader: Pass Format Error");
		}
	}

	void GLShaderBuilder::parseShader(std::string& content
		, std::string& rootPath
		, const char* regex
		, std::string& outContent)
	{
		std::regex regex_shader(regex);
		std::smatch result;
		if (std::regex_search(content, result, regex_shader))
		{
			std::string shader_content(result[1]);
			std::sregex_iterator end;

			//------------------------------------------------------------
			//
			//	删除注释
			//	包括//型 和 /**/型
			//
			std::regex regex_comment(R"(/\*[\s\S]*\*/|//.*)");
			shader_content = std::regex_replace(shader_content, regex_comment, "");

			//------------------------------------------------------------
			//
			//	分析Include
			//	1.分离出所有include
			//	2.删除所有include
			//	3.保存剩下的部分
			//
			std::regex regex_include(R"(#include\s*\"(\S+)\"\s*)");

			std::function<void(std::string&, std::unordered_set<uint64_t>&, std::vector<std::string>&, const std::filesystem::path&)> parse =
				[&parse, &regex_comment, &regex_include, &end](std::string& include_content
					, std::unordered_set<uint64_t>& check_includes
					, std::vector<std::string>& all_includes
					, const std::filesystem::path& rootPath)
			{
				include_content = std::regex_replace(include_content, regex_comment, "");

				//找出所有include
				std::vector<std::string> include_heads;
				for (auto struct_i = std::sregex_iterator(include_content.begin(), include_content.end(), regex_include); struct_i != end; struct_i++)
				{
					std::string include_name = (*struct_i)[1];
					include_heads.emplace_back(std::move(include_name));
				}

				//如果有include
				if (!include_heads.empty())
				{
					//遍历所有include 加载数据
					for (auto& file_path : include_heads)
					{
						std::filesystem::path sys_path(rootPath.string() + "/" + file_path);
						auto content = FileTool::loadText(sys_path.string());
						if (!content.empty())
						{
							auto hash_id = CityHash64(content.c_str(), content.size());
							if (!check_includes.contains(hash_id))
							{
								check_includes.emplace(hash_id);
								parse(content, check_includes, all_includes, sys_path.parent_path());
							}
						}
					}
				}

				all_includes.emplace_back(std::regex_replace(include_content, regex_include, ""));
			};

			std::unordered_set<uint64_t> check_includes;
			std::vector<std::string> all_includes;
			std::vector<std::string> include_heads;
			for (auto struct_i = std::sregex_iterator(shader_content.begin(), shader_content.end(), regex_include); struct_i != end; struct_i++)
			{
				std::string include_name = (*struct_i)[1];
				include_heads.emplace_back(std::move(include_name));
			}

			if (!include_heads.empty())
			{
				//删除所有include
				shader_content = std::regex_replace(shader_content, regex_include, "");

				//遍历所有include 加载数据
				for (auto& file_path : include_heads)
				{
					std::filesystem::path sys_path(rootPath + "/" + file_path);
					auto content = FileTool::loadText(sys_path.string());
					if (!content.empty())
					{
						auto hash_id = CityHash64(content.c_str(), content.size());
						if (!check_includes.contains(hash_id))
						{
							check_includes.emplace(hash_id);
							parse(content, check_includes, all_includes, sys_path.parent_path());
						}
					}
				}
			}

			if (!all_includes.empty())
			{
				std::string data;
				for (auto& s : all_includes)
				{
					data += s;
					data += "\n";
				}

				shader_content.insert(0, data);
			}

			//------------------------------------------------------------
			//
			// 分析struct中的arguments
			//
			struct ValueMetaData
			{
				struct Range
				{
					float min;
					float max;
				};

				std::string name;
				UniformBuildType buildType;
				std::string showName;

				union
				{
					Range range;
				};
			};

			struct StructMetaData
			{
				std::string name;
				//name, type
				std::unordered_map<std::string, std::string> arguments;
			};
			std::unordered_map<std::string_view, StructMetaData*> struct_map;

			//struct Name的解析规则
			std::regex regex_struct(R"(struct\s+(\w+)\s+\{[\w\W]+?\};)");
			//struct内容解析规则
			std::regex regex_struct_data(R"(struct\s+\w+\s+\{([\w\W]+)\};)");
			//type和name的解析规则
			std::regex regex_argument(R"((\w+)\s*(\w+)(\[\w*\]|\s*=\s*\S*)*;)");
			//这个专门解析数组
			//std::regex regex_argument(R"((\w+)\s*(\w+)(\[\d+\])*;)");

			//分析struct
			for (auto struct_i = std::sregex_iterator(shader_content.begin(), shader_content.end(), regex_struct); struct_i != end; struct_i++)
			{
				std::string struct_name = (*struct_i)[1];
				auto meta_data = new StructMetaData();
				meta_data->name = struct_name;
				struct_map[meta_data->name] = meta_data;

				//分析argument pair
				std::string struct_data = (*struct_i)[0];
				std::string arguments_data;
				for (auto struct_data_i = std::sregex_iterator(struct_data.begin(), struct_data.end(), regex_struct_data); struct_data_i != end; struct_data_i++)
				{
					//分析argument
					arguments_data = (*struct_data_i)[1];
					for (auto argument_i = std::sregex_iterator(arguments_data.begin(), arguments_data.end(), regex_argument); argument_i != end; argument_i++)
					{
						//[name,type]
						meta_data->arguments.emplace((*argument_i)[2], (*argument_i)[1]);
					}
				}
			}

			std::string cache, type;
			//分析Uniform
			std::regex regex_uniform(R"(uniform\s+(\w+)\s+(\w+)(\[\w*\]|\s*=\s*\S*)*;)");
			//这个专门解析数组
			//std::regex regex_uniform(R"(uniform\s+(\w+)\s+(\w+)(\[\d+\])*;)");
			for (auto uniform_i = std::sregex_iterator(shader_content.begin(), shader_content.end(), regex_uniform); uniform_i != end; uniform_i++)
			{
				type = (*uniform_i)[1];
				auto it = struct_map.find(type);
				if (it != struct_map.end())
				{
					auto meta_data = it->second;
					for (auto& pair : meta_data->arguments)
					{
						cache = std::string((*uniform_i)[2]) + "." + pair.first;
						if (cache.starts_with("TINY_"))
						{
							//组合struct中的uniform
							//mUniformSet.emplace(std::move(cache));
							mTinyUMap.emplace(std::move(cache), ContextMap::UniformTypeUMap[pair.second]);
						}
						else
						{
							//mUserSet.emplace(std::move(cache));
							mUserUMap.emplace(std::move(cache), ContextMap::UniformTypeUMap[pair.second]);
						}
					}
				}
				else
				{
					cache = (*uniform_i)[2];
					if (cache.starts_with("TINY_"))
					{
						//组合struct中的uniform
						//mUniformSet.emplace(std::move(cache));
						mTinyUMap.emplace(std::move(cache), ContextMap::UniformTypeUMap[(*uniform_i)[1]]);
					}
					else
					{
						//mUserSet.emplace(std::move(cache));
						mUserUMap.emplace(std::move(cache), ContextMap::UniformTypeUMap[(*uniform_i)[1]]);
					}
				}
			}

			for (auto& pair : struct_map)
			{
				delete pair.second;
			}

			shader_content.insert(0, " core\n\r");
			shader_content.insert(0, std::to_string(mConfigUMap["Version"].cast<int>()));
			shader_content.insert(0, "#version ");

			//------------------------------------------------------
			//
			//	写入shader头
			//
			outContent = std::move(shader_content);
		}
		else
		{
			//throw std::logic_error("GLShader: Shader Format Error");

			Log_Error("GLShader: Shader Format Error");
		}
	}
}
