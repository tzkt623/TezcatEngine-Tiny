#include "Core/Shader/ShaderParser.h"
#include "Core/Shader/Shader.h"
#include "Core/Debug/Debug.h"

#include "ThirdParty/Hash/city.h"
#include "Core/Manager/VertexBufferManager.h"

#include "Core/Shader/ShaderParserRule.h"

namespace tezcat::Tiny
{
	ShaderParser::ShaderParser()
	{

	}

	void ShaderParser::parse(const std::string& path)
	{
		auto root = file_path(path).parent_path().string();

		auto content = FileTool::loadText(path);
		this->removeComment(content);

		this->parseHeader(content);
		this->parseShaders(content, root);
	}

	void ShaderParser::parse(std::string& content, const std::string& path)
	{
		this->removeComment(content);
		auto root = file_path(path).parent_path().string();

		this->parseHeader(content);
		this->parseShaders(content, root);
	}

	bool ShaderParser::splitConfig(const std::string& content, std::string& config, std::string& suffix, const std::regex& regex)
	{
		//std::regex reg(regex);
		std::smatch result;
		if (std::regex_search(content, result, regex))
		{
			config = result[1];
			suffix = result.suffix();
			//			std::cout << result[1] << std::endl;
			return true;
		}

		return false;
	}

	bool ShaderParser::splitValue(std::string& content, std::unordered_map<std::string, Any>& map)
	{
		content = std::regex_replace(content, ShaderParserRule::GetTabAndEnter, "");

		std::smatch value_pair_result;
		std::string type, name, value, temp;

		//0表示全匹配
		//-1表示用匹配字符串(当前是;)当分隔符,分割原始字符串并保存,但是不保存匹配字符串(;)
		//>0表示匹配并保存第x个字符串
		for (auto i = std::sregex_token_iterator(content.begin(), content.end(), ShaderParserRule::SpliterSemicolon, -1); i != ShaderParserRule::EndTokenIterator; i++)
		{
			temp = std::move(i->str());
			if (std::regex_search(temp, value_pair_result, ShaderParserRule::GetValueInfo))
			{
				type = std::move(value_pair_result[1].str());
				name = std::move(value_pair_result[2].str());
				value = std::move(value_pair_result[3].str());

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
						TINY_LOG_ERROR("ShaderParser: Shader Param [bool]`s string must [true] or [false]");
					}
				}
			}
		}

		return !map.empty();
	}

	void ShaderParser::splitPasses(std::string& content, std::vector<std::string>& passArray)
	{
		std::sregex_iterator end;
		for (auto i = std::sregex_iterator(content.begin(), content.end(), ShaderParserRule::Tiny::GetPass); i != end; i++)
		{
			//std::string temp(i->str(1));
			passArray.emplace_back(std::move(i->str(1)));
		}
	}

	std::string ShaderParser::getName(const std::string& textToSave)
	{
		std::smatch result;
		if (std::regex_search(textToSave, result, ShaderParserRule::Tiny::GetNameInHead))
		{
			return { std::move(result.str(1)) };
		}

		return {};
	}

	void ShaderParser::parseHeader(std::string& content)
	{
		std::smatch result;
		if (std::regex_search(content, result, ShaderParserRule::Tiny::GetHead))
		{
			std::string temp(std::move(result.str(1)));
			//content = result.suffix();
			ShaderParser::splitValue(temp, mHeadUMap);
		}
	}

	void ShaderParser::parseShaders(std::string& content, std::string& rootPath)
	{
		std::string config, shader_content;
		if (splitConfig(content, config, shader_content, ShaderParserRule::Tiny::GetConfig))
		{
			ShaderParser::splitValue(config, mConfigUMap);
			if (!this->parseShader(shader_content
				, rootPath
				, ShaderParserRule::Tiny::GetVS
				, mVertexShader))
			{
				TINY_LOG_ERROR("ShaderParser: Shader Format Error");
			}

			if (!this->parseShader(shader_content
				, rootPath
				, ShaderParserRule::Tiny::GetFS
				, mFragShader))
			{
				TINY_LOG_ERROR("ShaderParser: Shader Format Error");
			}

			this->parseShader(shader_content
				, rootPath
				, ShaderParserRule::Tiny::GetGS
				, mGeometryShader);
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
		content = std::regex_replace(content, ShaderParserRule::GetComment, "");
	}

	bool ShaderParser::parseShader(std::string& content
		, std::string& rootPath
		, const std::regex& regex
		, std::string& outContent)
	{
		//std::regex regex_shader(regex);
		std::smatch result;
		if (std::regex_search(content, result, regex))
		{
			std::string shader_content(result[1]);

			this->splitInclude(shader_content, rootPath);
			this->splitUniformBuffer(shader_content);
			this->splitStruct(shader_content);
			this->writeShaderHead(shader_content);

			outContent = std::move(shader_content);
			return true;
		}

		return false;
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
		std::unordered_set<uint64_t> check_includes;
		std::vector<file_path> all_includes;
		std::vector<file_path> include_heads;
		for (auto struct_i = std::sregex_iterator(content.begin(), content.end(), ShaderParserRule::Tiny::GetIncludeFile); struct_i != ShaderParserRule::EndIterator; struct_i++)
		{
			file_path include_name(struct_i->str(1));
			include_heads.emplace_back(std::move(include_name));
		}

		if (!include_heads.empty())
		{
			//删除所有include
			content = std::regex_replace(content, ShaderParserRule::Tiny::GetIncludeFile, "");

			//遍历所有include 加载数据
			for (auto& head_file_path : include_heads)
			{
				file_path sys_path(rootPath / head_file_path);
				auto content = FileTool::loadText(sys_path.string());
				if (!content.empty())
				{
					auto hash_id = CityHash64(content.c_str(), content.size());
					if (!check_includes.contains(hash_id))
					{
						check_includes.emplace(hash_id);
						this->parseInclude(content, check_includes, all_includes, sys_path.parent_path());
					}
				}
			}
		}

		if (!all_includes.empty())
		{
			std::string data;
			for (auto& s : all_includes)
			{
				data += s.string();
				data += "\n";
			}

			content.insert(0, data);
		}
	}

	void ShaderParser::splitUniformBuffer(std::string& content)
	{
		constexpr int32_t index_name = 2;
		constexpr int32_t index_binding_index = 1;
		constexpr int32_t index_values = 3;
		//------------------------------------------------------------
		//
		// 分析Uniform Object
		//
		try
		{
			std::string name;
			for (auto uniform_object_i = std::sregex_iterator(content.begin(), content.end(), ShaderParserRule::UniformBuffer::GetOneBuffer); uniform_object_i != ShaderParserRule::EndIterator; uniform_object_i++)
			{
				auto layout_struct = std::make_unique<ShaderMetaDataLayoutStruct>();
				layout_struct->initBindingAttribute();

				layout_struct->name = uniform_object_i->str(index_name);
				layout_struct->attributBinding->bindingIndex = std::stoi(uniform_object_i->str(index_binding_index));

				std::string values(std::move(uniform_object_i->str(index_values)));
				for (auto it = std::sregex_iterator(values.begin(), values.end(), ShaderParserRule::UniformBuffer::SplitValue); it != ShaderParserRule::EndIterator; it++)
				{
					//分析成员名称并记录
					layout_struct->addMemeber(it->str(2), GraphicsConfig::UniformTypeUMap[it->str(1)]);
				}

				//auto [flag, buffer] = VertexBufferManager::createUniformBufferLayout(name);
				//if (flag)
				//{
				//	buffer->mName = std::move(name);
				//	buffer->mBindingIndex = std::stoi(uniform_object_i->str(index_binding_index));
				//
				//	//得到类中的所有成员
				//	std::string values(std::move(uniform_object_i->str(index_values)));
				//	for (auto it = std::sregex_iterator(values.begin(), values.end(), ShaderParserRule::UniformBuffer::SplitValue); it != ShaderParserRule::EndIterator; it++)
				//	{
				//		//分析成员名称并记录
				//		buffer->mSlot.emplace_back(it->str(1), -1, 0);
				//		auto member = std::make_shared<ShaderMetaDataMember>();
				//		member->name = it->str(1);
				//		layout_struct->memberMap.emplace(member->name, member);
				//	}
				//}
				mUBOMap.emplace(layout_struct->name, std::move(layout_struct));
			}

			content = std::regex_replace(content, ShaderParserRule::UniformBuffer::GetProperty, "");
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
		//分析并记录所有的struct
		//以供在后续解析中查找
		for (auto struct_i = std::sregex_iterator(content.begin(), content.end(), ShaderParserRule::Struct::GetOneStruct); struct_i != ShaderParserRule::EndIterator; struct_i++)
		{
			auto meta_data_struct = std::make_unique<ShaderMetaDataStruct>();
			meta_data_struct->structType = (*struct_i)[1];

			//分析argument pair
			auto& struct_match = (*struct_i)[2];
			std::string arg_type, arg_name, arg_count;

			for (auto argument_i = std::sregex_iterator(struct_match.first, struct_match.second, ShaderParserRule::Struct::GetMemberValue); argument_i != ShaderParserRule::EndIterator; argument_i++)
			{
				arg_type = argument_i->str(2);

				auto it = GraphicsConfig::UniformTypeUMap.find(arg_type);
				if (it != GraphicsConfig::UniformTypeUMap.end())
				{
					arg_name = argument_i->str(3);
					arg_count = argument_i->str(4);
					int array_count = 0;
					if (!arg_count.empty())
					{
						array_count = std::stoi(arg_count);
					}

					auto member = meta_data_struct->addMemeber(arg_name, it->second);
					member->arrayCount = array_count;
					if (((*argument_i)[1]).matched)
					{
						std::string attributes_str = std::regex_replace(argument_i->str(1), ShaderParserRule::GetBlank, "");
						this->parseAttribute(attributes_str, member);
					}
				}
				else
				{
					auto _struct = meta_data_struct->addStruct(arg_name);
					_struct->structType = arg_type;
				}

			}

			mStructUMap[meta_data_struct->structType] = std::move(meta_data_struct);
		}

		this->parseUniformValue(content);

		//删除约束
		content = std::regex_replace(content, ShaderParserRule::GetAllAttribute, "");
	}

	void ShaderParser::parseAttribute(const std::string& attributeMatch, ShaderMetaDataMember* member)
	{
		for (auto i = std::regex_iterator(attributeMatch.begin(), attributeMatch.end(), ShaderParserRule::GetOneAttribute); i != ShaderParserRule::EndIterator; i++)
		{
			std::string attribute_type = i->str(1);
			auto& attribute_value = (*i)[2];

			if (attribute_type == "Editor")
			{
				this->parseEditorAttribute(attribute_value, member);
			}
			else if (attribute_type == "Register")
			{
				auto attribute = std::make_unique<ShaderAttributeRegister>();

				std::smatch match_attribute_value;
				std::regex_match(attribute_value.first, attribute_value.second, match_attribute_value, ShaderParserRule::Uniform::GetAttributeValue);
				std::string type = match_attribute_value[1];
				if (type == "GlobalSlot")
				{
					attribute->globalIndex = std::stoi(match_attribute_value[2]);
				}

				member->attributeRegister = std::move(attribute);

				//mGlobalUniform.push_back(member);
			}
		}
	}

	void ShaderParser::parseUniformValue(std::string& content)
	{
		for (auto uniform_i = std::sregex_iterator(content.begin(), content.end(), ShaderParserRule::Uniform::GetOneUniform); uniform_i != ShaderParserRule::EndIterator; uniform_i++)
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
				//结构数据
				auto& ptr = it->second;
				//创建当前
				auto uniform_value = std::make_unique<ShaderMetaDataStruct>();
				uniform_value->structType = ptr->structType;
				uniform_value->name = uniform_match_name;
				uniform_value->arrayCount = array_count;


				//auto& members = ptr->getMembers();
				uniform_value->copyMembers(ptr.get());

				if (uniform_value->name.starts_with("TINY_"))
				{
					mTinyUMap.emplace(uniform_value->name, std::move(uniform_value));
				}
				else
				{
					mUserUMap.emplace(uniform_value->name, std::move(uniform_value));
				}
			}
			else
			{
				auto uniform_value = std::make_unique<ShaderMetaDataMember>();
				uniform_value->name = uniform_match_name;
				uniform_value->arrayCount = array_count;
				uniform_value->valueType = GraphicsConfig::UniformTypeUMap[uniform_type];

				if (((*uniform_i)[1]).matched)
				{
					std::string attributes_str = std::regex_replace(uniform_i->str(1), ShaderParserRule::GetBlank, "");
					this->parseAttribute(attributes_str, uniform_value.get());
				}
				//this->parseAttribute((*uniform_i)[1], uniform_value.get());

				if (uniform_value->name.starts_with("TINY_"))
				{
					//组合struct中的uniform
					mTinyUMap.emplace(uniform_value->name, std::move(uniform_value));
				}
				else
				{
					mUserUMap.emplace(uniform_value->name, std::move(uniform_value));
				}
			}
		}
	}

	void ShaderParser::parseInclude(std::string& include_content
		, std::unordered_set<uint64_t>& check_includes
		, std::vector<file_path>& all_includes
		, const file_path& rootPath)
	{
		//删除所有注释
		this->removeComment(include_content);

		//找出所有include
		std::vector<std::string> include_heads;
		for (auto struct_i = std::sregex_iterator(include_content.begin(), include_content.end(), ShaderParserRule::Tiny::GetIncludeFile); struct_i != ShaderParserRule::EndIterator; struct_i++)
		{
			std::string include_name = (*struct_i)[1];
			include_heads.emplace_back(std::move(include_name));
		}

		//如果有include
		if (!include_heads.empty())
		{
			//遍历所有include 加载数据
			for (auto& head_file_path : include_heads)
			{
				file_path sys_path(rootPath / head_file_path);
				auto content = FileTool::loadText(sys_path);
				if (!content.empty())
				{
					auto hash_id = CityHash64(content.c_str(), content.size());
					if (!check_includes.contains(hash_id))
					{
						check_includes.emplace(hash_id);
						this->parseInclude(content, check_includes, all_includes, sys_path.parent_path());
					}
				}
			}
		}

		//删掉所有include
		all_includes.emplace_back(std::move(std::regex_replace(include_content, ShaderParserRule::Tiny::GetIncludeFile, "")));
	}

	void ShaderParser::parseEditorAttribute(const std::ssub_match& attributeMatch, ShaderMetaDataMember* member)
	{
		auto attribute = std::make_unique<ShaderAttributeEditor>();

		//解析约束值的类型
		std::string type, context;
		for (auto i = std::regex_iterator(attributeMatch.first, attributeMatch.second, ShaderParserRule::Attribute::GetOneAttributeData); i != ShaderParserRule::EndIterator; i++)
		{
			type = i->str(1);
			if (type == "Name")
			{
				attribute->editorName = i->str(2);
				continue;
			}

			if (type == "Range")
			{
				context = i->str(2);
				attribute->rangeType = RangeType::Number;
				std::smatch match_range;
				std::regex_search(context, match_range, ShaderParserRule::Attribute::GetRangeValue);
				std::string min = match_range.str(1);
				std::string max = match_range.str(2);

				switch (member->valueType)
				{
				case UniformType::Float:
				case UniformType::Float2:
				case UniformType::Float3:
				case UniformType::Float4:
				{
					//float fmin = 0, fmax = 0;
					//std::from_chars(min.data(), min.data() + min.size(), fmin);
					//std::from_chars(max.data(), max.data() + max.size(), fmax);
					attribute->range.reset(new RangeFloat{ std::stof(min), std::stof(max) });
					break;
				}
				case UniformType::Int:
				case UniformType::Int2:
				case UniformType::Int3:
				case UniformType::Int4:
				{
					//int32_t fmin = 0, fmax = 0;
					//std::from_chars(min.data(), min.data() + min.size(), fmin);
					//std::from_chars(max.data(), max.data() + max.size(), fmax);
					attribute->range.reset(new RangeInt{ std::stoi(min), std::stoi(max) });
					break;
				}
				case UniformType::UInt:
				case UniformType::UInt2:
				case UniformType::UInt3:
				case UniformType::UInt4:
				{
					//uint32_t fmin = 0, fmax = 0;
					//std::from_chars(min.data(), min.data() + min.size(), fmin);
					//std::from_chars(max.data(), max.data() + max.size(), fmax);
					attribute->range.reset(new RangeUInt{ std::stoul(min), std::stoul(max) });
					break;
				}
				default:
					//attribute->range.reset(new RangeFloat{ 0, 0 });
					break;
				}

				continue;
			}

			if(type == "Color")
			{
				attribute->rangeType = RangeType::Color;
				continue;
			}
		}

		member->attributeEditor = std::move(attribute);
	}
	void ShaderParser::updateShaderConfig(Shader* shader)
	{
		for (auto& pair : mTinyUMap)
		{
			shader->registerTinyUniform(pair.second.get());
		}

		shader->resizeUserUniformArray(mUserUMap.size());
		for (auto& pair : mUserUMap)
		{
			shader->registerUserUniform(pair.second.get());
		}

		for (auto& pair : mUBOMap)
		{
			auto [flag, buffer] = VertexBufferManager::createUniformBufferLayout(pair.first.data());
			if (flag == FlagCreate::Succeeded)
			{
				auto& attribute = pair.second;
				buffer->mName = attribute->name;
				buffer->mBindingIndex = attribute->attributBinding->bindingIndex;
				for (auto& member : attribute->getMembers())
				{
					buffer->mSlot.emplace_back(member->name, -1, 0);
				}
			}
		}

		for (auto global : mGlobalUniform)
		{
			//auto info = global->getInfo<ShaderMemberInfo>();
			//auto index = info->attributeRegister->globalIndex;
			//GlobalSlotManager::registerSlot();
			//shader->registerGlobalUniform(index, )
		}

		shader->setName(mHeadUMap["Name"].cast<std::string>());
		shader->setVersion(mConfigUMap["Version"].cast<int>());

		//OrderID
		auto it = mConfigUMap.find("OrderID");
		if (it != mConfigUMap.end())
		{
			shader->setSortingOffset(it->second.cast<int>());
		}
		else
		{
			shader->setSortingOffset(0);
		}

		//Queue
		it = mConfigUMap.find("Queue");
		if (it != mConfigUMap.end())
		{
			shader->setRenderQueue(GraphicsConfig::QueueMap[it->second.cast<std::string>()]);
		}
		else
		{
			shader->setRenderQueue(PipelineQueueType::Opaque);
		}

		//LightMode
		it = mConfigUMap.find("LightMode");
		if (it != mConfigUMap.end())
		{
			shader->setLightMode(GraphicsConfig::LightModeMap[it->second.cast<std::string>()]);
		}
		else
		{
			shader->setLightMode(LightMode::Unlit);
		}

		//DepthTest
		it = mConfigUMap.find("DepthTest");
		if (it != mConfigUMap.end())
		{
			shader->setDepthTest(GraphicsConfig::DepthTestMap[it->second.cast<std::string>()]);
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
			shader->setBlendFunction(GraphicsConfig::BlendMap[mConfigUMap["BlendSrc"].cast<std::string>()]
				, GraphicsConfig::BlendMap[mConfigUMap["BlendTar"].cast<std::string>()]);
		}
		else
		{
			shader->setBlend(false);
		}

		//CullFace
		it = mConfigUMap.find("CullFace");
		if (it != mConfigUMap.end())
		{
			shader->setCullFace(GraphicsConfig::CullFaceMap[it->second.cast<std::string>()]);
		}
	}

	void ShaderParser::writeShaderHead(std::string& content)
	{
		//------------------------------------------------------
		//
		//	写入shader头
		//
		//content.insert(0, "#pragma optimize(off)\n");

		content = std::format("#version {} core\n{}", mConfigUMap["Version"].cast<int>(), content);
	}

}

