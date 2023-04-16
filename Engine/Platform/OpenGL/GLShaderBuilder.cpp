#include "GLShaderBuilder.h"
#include "GLShader.h"
#include "GLHead.h"
#include "Core/Head/Context.h"
#include "Core/Manager/ShaderManager.h"
#include "Core/Pipeline/PipelineQueue.h"
#include "Core/Shader/ShaderPackage.h"


namespace tezcat::Tiny::GL
{
	bool splitConfig(const std::string& content, std::string& config, std::string& suffix, const char* regex)
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

	bool splitValue(std::string& content, std::unordered_map<std::string, Any>& map)
	{
		std::regex regex_empty(R"(\t|\r|\n)");
		content = std::regex_replace(content, regex_empty, "");

		std::regex regex_spliter(";");
		std::sregex_token_iterator end;

		std::regex regex_value_pair(R"((\w+)\s(\w+)\s=\s([\s\S]+))");
		std::smatch value_pair_result;

		for (auto i = std::sregex_token_iterator(content.begin(), content.end(), regex_spliter, -1); i != end; i++)
		{
			std::string temp(std::move((*i).str()));
			if (std::regex_search(temp, value_pair_result, regex_value_pair))
			{
				std::string type(std::move(value_pair_result[1].str()));
				std::string name(std::move(value_pair_result[2].str()));
				std::string value(std::move(value_pair_result[3].str()));

				if (type == "str")
				{
					map.emplace(name, value);
				}
				else if (type == "int")
				{
					map.emplace(name, std::stoi(value));
				}
				else if (type == "float")
				{
					map.emplace(name, std::stof(value));
				}
				else if (type == "bool")
				{
					if (value == "true")
					{
						map.emplace(name, true);
					}
					else if (value == "false")
					{
						map.emplace(name, false);
					}
					else
					{
						throw std::logic_error("GLShader: Shader Param [bool]`s string must [true] or [false]");
					}
				}
			}
		}

		return !map.empty();
	}

	//----------------------------------------------------------------------
	//
	//	ShaderBuilder
	//
	GLShaderBuilder::GLShaderBuilder()
	{
		mShaderIDs.reserve(4);
	}

	GLShaderBuilder::~GLShaderBuilder()
	{
		for (auto id : mShaderIDs)
		{
			std::cout << "ShaderBuilder : " << id << "Deleted" << std::endl;
			glDeleteShader(id);
		}
		mShaderIDs.clear();
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

			this->splitPackage(data);
		}
		io.close();
		return shader;
	}

	ShaderPackage* GLShaderBuilder::splitPackage(std::string& content)
	{
		std::string head, passes;
		if (splitConfig(content, head, passes, R"(#TINY_HEAD_BEGIN\s*\{\s*([\s\S]*)\}\s*#TINY_HEAD_END)"))
		{
			auto pack = this->parsePackageHead(head);
			this->splitPasses(pack, passes);
			pack->apply();

			return pack;
		}

		throw std::logic_error("Build Shader Error!");
	}

	ShaderPackage* GLShaderBuilder::parsePackageHead(std::string& content)
	{
		std::unordered_map<std::string, Any> map;
		splitValue(content, map);
		auto pack = new ShaderPackage();
		pack->setConfig(map);
		return pack;
	}

	void GLShaderBuilder::splitPasses(ShaderPackage* pack, std::string& content)
	{
		std::regex pattern(R"(#TINY_PASS_BEGIN\s*\{\s*([\s\S]*?)\}\s*#TINY_PASS_END)");
		std::sregex_iterator end;
		for (auto i = std::sregex_iterator(content.begin(), content.end(), pattern); i != end; i++)
		{
			UniformID::USet set;

			std::string temp((*i)[1]);
			auto shader = new GLShader();
			this->parseShaders(shader, temp, set);
			shader->apply(set);

			pack->addShader(shader);
		}
	}

	void GLShaderBuilder::parseShaders(GLShader* shader, std::string& content, UniformID::USet& uniformArray)
	{
		std::string config, shader_content;
		if (splitConfig(content, config, shader_content, R"(#TINY_CFG_BEGIN\s*\{\s*([\s\S]*)\}\s*#TINY_CFG_END)"))
		{
			this->parseShaderConfig(shader, config);
			this->parseShader(shader
				, shader_content
				, R"(#TINY_VS_BEGIN\s*\{\s*([\s\S]*)\}\s*#TINY_VS_END)"
				, GL_VERTEX_SHADER
				, uniformArray);
			this->parseShader(shader
				, shader_content
				, R"(#TINY_FS_BEGIN\s*\{\s*([\s\S]*)\}\s*#TINY_FS_END)"
				, GL_FRAGMENT_SHADER
				, uniformArray);
		}
		else
		{
			throw std::logic_error("GLShader: Pass Format Error");
		}
	}

	void GLShaderBuilder::parseShaderConfig(GLShader* shader, std::string& content)
	{
		std::unordered_map<std::string, Any> map;
		if (splitValue(content, map))
		{
			shader->setVersion(map["Version"].cast<int>());

			//Name
			auto it = map.find("Name");
			if (it != map.end())
			{
				shader->setName(it->second.cast<std::string>());
			}

			//OrderID
			it = map.find("OrderID");
			if (it != map.end())
			{
				shader->setOrderID(it->second.cast<int>());
			}
			else
			{
				shader->setOrderID(0);
			}

			//Queue
			it = map.find("Queue");
			if (it != map.end())
			{
				shader->setRenderQueue(PipelineQueue::getQueue(it->second.cast<std::string>()));
			}
			else
			{
				shader->setRenderQueue(PipelineQueue::Queue::None);
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

			//Lighting
			it = map.find("Lighting");
			if (it != map.end())
			{
				shader->setLighting(it->second.cast<bool>());
			}
			else
			{
				shader->setLighting(false);
			}
		}
		else
		{
			throw std::logic_error("GLShader: Shader CFG Error");
		}
	}


	void GLShaderBuilder::parseShader(GLShader* shader, std::string& content, const char* regex, uint32_t shaderType, UniformID::USet& uniformArray)
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
			//
			//
			std::vector<std::string> include_ary;
			std::regex regex_include(R"(#include\s*\"(\S+)\"\s*)");
			for (auto struct_i = std::sregex_iterator(shader_content.begin(), shader_content.end(), regex_include); struct_i != end; struct_i++)
			{
				std::string include_name = (*struct_i)[1];
				include_ary.emplace_back(include_name);
			}

			if (!include_ary.empty())
			{
				//组合shader
				shader_content = std::regex_replace(shader_content, regex_include, "");

				std::string inlcude_data;
				for (auto& s : include_ary)
				{
					inlcude_data += ShaderMgr::getInstance()->getIncludeContent(s);
				}

				shader_content = inlcude_data + shader_content;
			}

			//------------------------------------------------------------
			//
			// 分析struct中的arguments
			//
			struct StructMetaData
			{
				std::string name;
				std::unordered_map<std::string, std::string> arguments;
			};
			std::unordered_map<std::string, StructMetaData*> struct_map;

			//struct Name的解析规则
			std::regex regex_struct(R"(struct\s+(\w+)\s+\{[\w\W]+?\};)");
			//struct内容解析规则
			std::regex regex_struct_data(R"(struct\s+\w+\s+\{([\w\W]+)\};)");
			//type和name的解析规则
			std::regex regex_argument(R"((\w+)\s*(\w+);)");

			//分析struct
			for (auto struct_i = std::sregex_iterator(shader_content.begin(), shader_content.end(), regex_struct); struct_i != end; struct_i++)
			{
				std::string struct_name = (*struct_i)[1];
				auto meta_data = new StructMetaData();
				meta_data->name = struct_name;
				struct_map[struct_name] = meta_data;

				//分析argument pair
				std::string struct_data = (*struct_i)[0];
				for (auto struct_data_i = std::sregex_iterator(struct_data.begin(), struct_data.end(), regex_struct_data); struct_data_i != end; struct_data_i++)
				{
					//分析argument
					std::string arguments_data = (*struct_data_i)[1];
					for (auto argument_i = std::sregex_iterator(arguments_data.begin(), arguments_data.end(), regex_argument); argument_i != end; argument_i++)
					{
						//[name,type]
						meta_data->arguments.emplace((*argument_i)[2], (*argument_i)[1]);
					}
				}
			}

			//分析Uniform
			std::regex regex_uniform(R"(uniform\s+(\w+)\s+(\w+)(?=[\s\S]*;))");
			for (auto i = std::sregex_iterator(shader_content.begin(), shader_content.end(), regex_uniform); i != end; i++)
			{
				std::string type = (*i)[1];
				auto it = struct_map.find(type);
				if (it != struct_map.end())
				{
					auto meta_data = it->second;
					for (auto& pair : meta_data->arguments)
					{
						//组合struct中的uniform
						uniformArray.emplace(std::string((*i)[2]) + "." + pair.first);
					}
				}
				else
				{
					uniformArray.emplace((*i)[2]);
				}
			}

			for (auto& pair : struct_map)
			{
				delete pair.second;
			}

			//------------------------------------------------------
			//
			//	写入shader头
			//
			//
			shader_content = "#version " + std::to_string(shader->getVersion()) + " core\n\r" + shader_content;
			switch (shaderType)
			{
			case GL_VERTEX_SHADER:
				this->loadFromData(shader, shader_content.c_str(), GL_VERTEX_SHADER);
				break;
			case GL_FRAGMENT_SHADER:
				this->loadFromData(shader, shader_content.c_str(), GL_FRAGMENT_SHADER);
				break;
			default:
				break;
			}
		}
		else
		{
			throw std::logic_error("GLShader: Shader Format Error");
		}
	}

	void GLShaderBuilder::loadFromData(GLShader* shader, const char* data, uint32_t shaderType)
	{
		auto shader_id = glCreateShader(shaderType);
		glShaderSource(shader_id, 1, &data, nullptr);
		glCompileShader(shader_id);

		int success;
		char infoLog[512];
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader_id, 512, nullptr, infoLog);
			switch (shaderType)
			{
			case GL_VERTEX_SHADER:
				std::cout << "GLShader [" + shader->getName() + "]: [VERTEX] COMPILATION_FAILED > " << infoLog << std::endl;
				break;
			case GL_FRAGMENT_SHADER:
				std::cout << "GLShader [" + shader->getName() + "]: [FRAGMENT] COMPILATION_FAILED > " << infoLog << std::endl;
				break;
			default:
				break;
			}
		}

		mShaderIDs.push_back(shader_id);
		shader->attachShader(shader_id);
	}

	ShaderPackage* GLShaderCreator::create(const std::string& filePath)
	{
		auto data = FileTool::loadText(filePath);
		GLShaderBuilder builder;
		return builder.splitPackage(data);
	}
}
