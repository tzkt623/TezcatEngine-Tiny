#include "Core/Shader/ShaderParser.h"
#include "Core/Shader/Shader.h"
#include "Core/Debug/Debug.h"

#include "ThirdParty/Hash/city.h"
#include "Core/Manager/VertexBufferManager.h"

namespace tezcat::Tiny
{
	ShaderParser::ShaderParser()
		: regex_remove_constraint(R"(\[\w+\(.*\)\]\s*)")
		, regex_comment(R"(/\*[\s\S]*\*/|//.*)")
		, end()
	{

	}

	bool ShaderParser::splitConfig(const std::string& content, std::string& config, std::string& suffix, const char* regex)
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

	bool ShaderParser::splitValue(std::string& content
		, std::unordered_map<std::string, Any>& map)
	{
		std::regex regex_empty(R"(\t|\r|\n)");
		content = std::regex_replace(content, regex_empty, "");

		std::regex regex_spliter(";");
		std::sregex_token_iterator end;

		std::regex regex_value_pair(R"((\w+)\s(\w+)\s=\s([\s\S]+))");
		std::smatch value_pair_result;

		std::string type, name, value, temp;

		//0表示全匹配
		//-1表示用匹配字符串(当前是;)当分隔符,分割原始字符串并保存,但是不保存匹配字符串(;)
		//>0表示匹配并保存第x个字符串
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
						TINY_LOG_ERROR("GLShader: Shader Param [bool]`s string must [true] or [false]");
						//throw std::logic_error("GLShader: Shader Param [bool]`s string must [true] or [false]");
					}
				}
			}
		}

		return !map.empty();
	}

	void ShaderParser::splitPasses(std::string& content, std::vector<std::string>& passArray)
	{
		std::regex pattern(R"(#TINY_PASS_BEGIN\s*\{\s*([\s\S]*?)\}\s*#TINY_PASS_END)");
		std::sregex_iterator end;
		for (auto i = std::sregex_iterator(content.begin(), content.end(), pattern); i != end; i++)
		{
			//UniformID::USet set;

			std::string temp((*i)[1]);
			passArray.push_back(std::move(temp));
		}
	}

	std::string ShaderParser::getName(const std::string& textToSave)
	{
		std::regex rg_find_name(R"(#TINY_HEAD_BEGIN\s*\{\s*([\s\S]*)\}\s*#TINY_HEAD_END)");
		std::sregex_token_iterator it(textToSave.begin(), textToSave.end(), rg_find_name, 1);
		std::string content(it->str());
		std::unordered_map<std::string, Any> map;
		ShaderParser::splitValue(content, map);
		content = map["Name"].cast<std::string>();

		return content;
	}

	void ShaderParser::parseHeader(std::string& content)
	{
		std::regex regex(R"(#TINY_HEAD_BEGIN\s*\{\s*([\s\S]*)\}\s*#TINY_HEAD_END)");
		std::smatch result;
		if (std::regex_search(content, result, regex))
		{
			std::string temp = result[1];
			content = result.suffix();
			ShaderParser::splitValue(temp, mHeadUMap);
		}
	}

	void ShaderParser::parseShaders(std::string& content, std::string& rootPath)
	{
		std::string config, shader_content;
		if (splitConfig(content, config, shader_content, R"(#TINY_CFG_BEGIN\s*\{\s*([\s\S]*)\}\s*#TINY_CFG_END)"))
		{
			ShaderParser::splitValue(config, mConfigUMap);
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
			throw std::logic_error("ShaderParser: Pass Format Error");
		}
	}

	void ShaderParser::removeComment(std::string& content)
	{
		//------------------------------------------------------------
		//
		//	删除注释
		//	包括//型 和 /**/型
		//
		content = std::regex_replace(content, regex_comment, "");
	}

	void ShaderParser::parseShader(std::string& content
		, std::string& rootPath
		, const char* regex
		, std::string& outContent)
	{
		std::regex regex_shader(regex);
		std::smatch result;
		if (std::regex_search(content, result, regex_shader))
		{
			std::string shader_content(result[1]);

			this->removeComment(shader_content);
			this->splitInclude(shader_content, rootPath);
			this->splitUniformBuffer(shader_content);
			this->splitStruct(shader_content);
			this->writeShaderHead(shader_content);

			outContent = std::move(shader_content);
		}
		else
		{
			//throw std::logic_error("GLShader: Shader Format Error");

			TINY_LOG_ERROR("ShaderParser: Shader Format Error");
		}
	}


	void ShaderParser::splitInclude(std::string& content, const std::string& rootPath)
	{
		//------------------------------------------------------------
		//
		//	分析Include
		//	1.分离出所有include
		//	2.删除所有include
		//	3.保存剩下的部分
		//
		std::regex regex_include(R"(#include\s*\"(\S+)\"\s*)");
		//std::regex regex_comment(R"(/\*[\s\S]*\*/|//.*)");
		//std::sregex_iterator end;

		std::function<void(std::string&, std::unordered_set<uint64_t>&, std::vector<std::string>&, const std::filesystem::path&)> parse =
			[this, &parse, &regex_include](std::string& include_content
				, std::unordered_set<uint64_t>& check_includes
				, std::vector<std::string>& all_includes
				, const std::filesystem::path& rootPath)
			{
				//删除所有注释
				this->removeComment(include_content);

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

				//删掉所有include
				all_includes.emplace_back(std::move(std::regex_replace(include_content, regex_include, "")));
			};

		std::unordered_set<uint64_t> check_includes;
		std::vector<std::string> all_includes;
		std::vector<std::string> include_heads;
		for (auto struct_i = std::sregex_iterator(content.begin(), content.end(), regex_include); struct_i != end; struct_i++)
		{
			std::string include_name = (*struct_i)[1];
			include_heads.emplace_back(std::move(include_name));
		}

		if (!include_heads.empty())
		{
			//删除所有include
			content = std::regex_replace(content, regex_include, "");

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

			content.insert(0, data);
		}
	}

	void ShaderParser::splitUniformBuffer(std::string& content)
	{
		//------------------------------------------------------------
		//
		// 分析Uniform Object
		//
		try
		{
			std::regex regex_uniform_object(R"(\[\s*Binding\s*=\s*(\d)\s*\]\s*layout\(std140\)\s+uniform\s+(\w+)\s*\{([\s\S]*?)\};)");
			std::regex regex_remove_constraint(R"(\[\s*Binding\s*=\s*\d\s*\]\s)");
			std::regex regex_split_value(R"(\w+\s+(\w+)[\[*\d\]]*;)");

			std::string name;
			for (auto uniform_object_i = std::sregex_iterator(content.begin(), content.end(), regex_uniform_object); uniform_object_i != end; uniform_object_i++)
			{
				name = (*uniform_object_i)[2];
				auto [flag, buffer] = VertexBufferManager::createUniformBufferLayout(name);
				if (flag)
				{
					buffer->mName = std::move(name);
					buffer->mBindingIndex = std::stoi((*uniform_object_i)[1]);

					std::string values = (*uniform_object_i)[3];
					for (auto it = std::sregex_iterator(values.begin(), values.end(), regex_split_value); it != end; it++)
					{
						buffer->mSlot.emplace_back((*it)[1], -1, 0);
						//buffer->pushLayout((*it)[1], -1, 0);
						//buffer->members.emplace((*it)[1], -1);
					}
				}
				mUBOMap.emplace(buffer->mName, buffer->mBindingIndex);
			}

			content = std::regex_replace(content, regex_remove_constraint, "");
		}
		catch (const std::regex_error& e)
		{
			std::cout << "regex_error caught: " << e.what() << '\n';
			if (e.code() == std::regex_constants::error_brack)
			{
				std::cout << "The code was error_brack\n";
			}
		}
	}

	void ShaderParser::splitStruct(std::string& content)
	{
		//------------------------------------------------------------
		//
		// 分析struct中的arguments
		//
		//struct Name的解析规则
		//没有+后面的?就会进入贪婪模式匹配到所有struct甚至一堆无用的数据
		std::regex regex_struct(R"(struct\s+(\w+)\s*\{([\w\W]+?)\s*\};)");
		//解析带约束的argument
		std::regex regex_constraint_argument(R"((?:(\[\w+\(.*\)\])\s*)*(\w+)\s+(\w+)\[?(\d*)\]?[\s\S]*?;)");
		//解析约束
		std::regex regex_constraint(R"(\[(\w+\(.*?\))\]\s*)");
		//解析约束的名称和值
		std::regex regex_constraint_name_value(R"((\w+)\((.*)\))");
		//解析约束的值
		std::regex regex_constraint_value(R"((\w+)\s*,?\s*(.*))");
		//移除约束条件
		//std::regex regex_remove_constraint(R"(\[\w+\(.*\)\]\s*)");
		//移除空白
		std::regex regex_space(R"(\s)");

		auto progress_constraint = [&regex_constraint_name_value, &regex_constraint_value, &regex_constraint, &regex_space]
		(const std::ssub_match& match_constraints, UniformType& utype)
			{
				std::smatch match_constraint;
				std::regex_match(match_constraints.first, match_constraints.second, match_constraint, regex_constraint);

				//解析约束的名称和内容
				std::smatch match_constraint_name_value;
				std::regex_match(match_constraint[1].first, match_constraint[1].second, match_constraint_name_value, regex_constraint_name_value);
				//std::string constraint_name = match_constraint_name_value[1];
				//std::string constraint_value = match_constraint_name_value[2];

				//解析约束值的类型
				std::smatch match_constraint_value;
				std::regex_match(match_constraint_name_value[2].first, match_constraint_name_value[2].second, match_constraint_value, regex_constraint_value);
				std::string show_name = match_constraint_value[1];
				std::string range = match_constraint_value[2];

				//删除空白
				show_name = std::regex_replace(show_name, regex_space, "");
				range = std::regex_replace(range, regex_space, "");

				ShaderConstraint constraint = ShaderConstraint::Error;
				size_t pos;
				std::shared_ptr<BaseRange> rangePtr;
				if ((pos = range.find_first_of("Range")) != range.npos)
				{
					constraint = ShaderConstraint::Range;

					range = range.substr(pos + 5, range.size() - pos);
					range.erase(range.begin());
					range.erase(range.end() - 1);

					pos = range.find_first_of(',');
					std::string min = range.substr(0, pos);
					std::string max = range.substr(pos + 1, range.size() - pos - 1);

					switch (utype)
					{
					case UniformType::Float:
					case UniformType::Float2:
					case UniformType::Float3:
					case UniformType::Float4:
						rangePtr.reset(new RangeFloat{ std::stof(min), std::stof(max) });
						break;
					case UniformType::Int:
					case UniformType::Int2:
					case UniformType::Int3:
					case UniformType::Int4:
						rangePtr.reset(new RangeInt{ std::stoi(min), std::stoi(max) });
						break;
					case UniformType::UInt:
					case UniformType::UInt2:
					case UniformType::UInt3:
					case UniformType::UInt4:
						rangePtr.reset(new RangeUInt{ std::stoul(min), std::stoul(max) });
						break;
					default:
						break;
					}
				}
				else if ((pos = range.find_first_of("Color")) != range.npos)
				{
					constraint = ShaderConstraint::Color;
				}
				else
				{
					rangePtr.reset(new RangeFloat{ 0.0f, 0.0f });
				}

				return std::tuple{ show_name, constraint, rangePtr };
			};

		//分析struct
		for (auto struct_i = std::sregex_iterator(content.begin(), content.end(), regex_struct); struct_i != end; struct_i++)
		{
			std::shared_ptr<ArgMetaData> meta_data(new ArgMetaData());
			auto struct_info = meta_data->createInfo<ArgStructInfo>(UniformType::Struct);
			struct_info->structName = (*struct_i)[1];
			meta_data->valueName = (*struct_i)[1];
			mStructUMap[meta_data->valueName] = meta_data;

			//分析argument pair
			auto& struct_match = (*struct_i)[2];

			for (auto argument_i = std::sregex_iterator(struct_match.first, struct_match.second, regex_constraint_argument); argument_i != end; argument_i++)
			{
				std::string arg_type = (*argument_i)[2];
				std::string arg_name = (*argument_i)[3];
				std::string arg_count = (*argument_i)[4];
				int array_count = 0;
				if (!arg_count.empty())
				{
					array_count = std::stoi(arg_count);
				}

				std::shared_ptr<ArgMetaData> member(new ArgMetaData());
				member->valueName = arg_name;
				member->valueCount = array_count;

				auto it = ContextMap::UniformTypeUMap.find(arg_type);
				if (it != ContextMap::UniformTypeUMap.end())
				{
					auto [show_name, constraint, rangePtr] = progress_constraint((*argument_i)[1], it->second);

					auto member_info = member->createInfo<ArgMemberInfo>(it->second);
					member_info->editorName = show_name;
					member_info->constraint = constraint;
					member_info->range = rangePtr;
				}
				else
				{
					auto member_info = member->createInfo<ArgStructInfo>(UniformType::Struct);
					member_info->structName = arg_type;
				}

				//[name,type]
				struct_info->members.emplace_back(member);
			}
		}

		//------------------------------------------------------------
		//
		// 分析Uniform
		//
		std::regex regex_uniform(R"((?:(\[\w+\(.*\)\])\s*)*uniform\s+(\w+)\s+(\w+)\[?(\d*)\]?[\s\S]*?;)");

		for (auto uniform_i = std::sregex_iterator(content.begin(), content.end(), regex_uniform); uniform_i != end; uniform_i++)
		{
			auto& uniform_match_type = (*uniform_i)[2];
			auto& uniform_match_name = (*uniform_i)[3];
			auto& uniform_match_array = (*uniform_i)[4];

			std::string str_array = uniform_match_array;
			int array_count = 0;
			if (!str_array.empty())
			{
				array_count = std::stoi(str_array);
			}

			//查找是否是结构体
			std::string uniform_type = uniform_match_type;
			//查找是否是结构类型
			auto it = mStructUMap.find(uniform_type);
			//如果是结构类型,转到处理结构类型的方式
			if (it != mStructUMap.end())
			{
				std::shared_ptr<ArgMetaData> uniform_value = std::make_shared<ArgMetaData>();
				auto uniform_info = uniform_value->createInfo<ArgStructInfo>(it->second->valueType);
				uniform_value->valueName = uniform_match_name;
				uniform_value->valueCount = array_count;

				uniform_info->structName = it->second->getInfo<ArgStructInfo>()->structName;
				auto& members = it->second->getInfo<ArgStructInfo>()->members;
				uniform_info->members.assign(members.begin(), members.end());

				if (uniform_value->valueName.starts_with("TINY_"))
				{
					mTinyUMap.emplace(uniform_value->valueName, uniform_value);
				}
				else
				{
					mUserUMap.emplace(uniform_value->valueName, uniform_value);
				}
			}
			else
			{
				std::shared_ptr<ArgMetaData> uniform_value = std::make_shared<ArgMetaData>();
				auto uniform_info = uniform_value->createInfo<ArgMemberInfo>(ContextMap::UniformTypeUMap[uniform_match_type]);
				uniform_value->valueName = uniform_match_name;
				uniform_value->valueCount = array_count;

				auto [show_name, constraint, rangePtr] = progress_constraint((*uniform_i)[1], uniform_value->valueType);

				uniform_info->editorName = show_name;
				uniform_info->constraint = constraint;
				uniform_info->range = rangePtr;

				if (uniform_value->valueName.starts_with("TINY_"))
				{
					//组合struct中的uniform
					mTinyUMap.emplace(uniform_value->valueName, uniform_value);
				}
				else
				{
					mUserUMap.emplace(uniform_value->valueName, uniform_value);
				}
			}
		}

		//删除约束
		content = std::regex_replace(content, regex_remove_constraint, "");
	}

	void ShaderParser::writeShaderHead(std::string& content)
	{
		//------------------------------------------------------
		//
		//	写入shader头
		//
		//content.insert(0, "#pragma optimize(off)\n");
		content.insert(0, " core\n");
		content.insert(0, std::to_string(mConfigUMap["Version"].cast<int>()));
		content.insert(0, "#version ");
	}

	void ShaderParser::parse(const std::string& path)
	{
		auto root = std::filesystem::path(path).parent_path().string();

		auto content = FileTool::loadText(path);
		this->parseHeader(content);
		this->parseShaders(content, root);
	}

	void ShaderParser::parse(std::string& content, const std::string& path)
	{
		auto root = std::filesystem::path(path).parent_path().string();

		this->parseHeader(content);
		this->parseShaders(content, root);
	}

	void ShaderParser::updateShaderConfig(Shader* shader)
	{
		shader->setName(mHeadUMap["Name"].cast<std::string>());
		shader->setVersion(mConfigUMap["Version"].cast<int>());

		//OrderID
		auto it = mConfigUMap.find("OrderID");
		if (it != mConfigUMap.end())
		{
			shader->setOrderID(it->second.cast<int>());
		}
		else
		{
			shader->setOrderID(2000);
		}

		//Queue
		it = mConfigUMap.find("Queue");
		if (it != mConfigUMap.end())
		{
			shader->setRenderQueue(ContextMap::QueueMap[it->second.cast<std::string>()]);
		}
		else
		{
			shader->setRenderQueue(Queue::None);
		}

		//LightMode
		it = mConfigUMap.find("LightMode");
		if (it != mConfigUMap.end())
		{
			shader->setLightMode(ContextMap::LightModeMap[it->second.cast<std::string>()]);
		}
		else
		{
			shader->setLightMode(LightMode::Unlit);
		}

		//DepthTest
		it = mConfigUMap.find("DepthTest");
		if (it != mConfigUMap.end())
		{
			shader->setDepthTest(ContextMap::DepthTestMap[it->second.cast<std::string>()]);
			//ZWrite
			it = mConfigUMap.find("ZWrite");
			if (it != mConfigUMap.end())
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
		it = mConfigUMap.find("Blend");
		if (it != mConfigUMap.end() && it->second.cast<bool>())
		{
			shader->setBlend(true);
			shader->setBlendFunction(ContextMap::BlendMap[mConfigUMap["BlendSrc"].cast<std::string>()]
				, ContextMap::BlendMap[mConfigUMap["BlendTar"].cast<std::string>()]);
		}
		else
		{
			shader->setBlend(false);
		}

		//CullFace
		it = mConfigUMap.find("CullFace");
		if (it != mConfigUMap.end())
		{
			shader->setCullFace(ContextMap::CullFaceMap[it->second.cast<std::string>()]);
		}
	}

}

