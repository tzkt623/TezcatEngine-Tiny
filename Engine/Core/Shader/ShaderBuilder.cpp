#include "ShaderBuilder.h"
#include "Shader.h"
#include "../Manager/ShaderManager.h"
#include "../Head/GLHead.h"
#include "ShaderPackage.h"


namespace tezcat::Tiny::Core
{
	bool splitConfig(const std::string& content, std::string& config, std::string& suffix, const char* regex)
	{
		std::regex reg(regex);
		std::smatch result;
		if (std::regex_search(content, result, reg))
		{
			config = result[1];
			suffix = result.suffix();
			std::cout << result[1] << std::endl;
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
				if (value_pair_result[1] == "str")
				{
					std::string t = value_pair_result[3];
					map.emplace(value_pair_result[2], t);
				}
				else if (value_pair_result[1] == "int")
				{
					map.emplace(value_pair_result[2], std::stoi(value_pair_result[3]));
				}
				else if (value_pair_result[1] == "float")
				{
					map.emplace(value_pair_result[2], std::stof(value_pair_result[3]));
				}
				else if (value_pair_result[1] == "bool")
				{
					if (value_pair_result[3] == "true")
					{
						map.emplace(value_pair_result[2], true);
					}
					else
					{
						map.emplace(value_pair_result[2], false);
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
	ShaderBuilder::ShaderBuilder()
	{
		m_ShaderIDs.reserve(4);
	}

	ShaderBuilder::~ShaderBuilder()
	{
		for (auto id : m_ShaderIDs)
		{
			std::cout << "ShaderBuilder : " << id << "Deleted" << std::endl;
			glDeleteShader(id);
		}
		m_ShaderIDs.clear();
	}

	Shader* ShaderBuilder::loadFromFile(const char* filePath)
	{
		Shader* shader = nullptr;
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

	void ShaderBuilder::splitPackage(std::string& content)
	{
		std::string head, passes;
		if (splitConfig(content, head, passes, R"(#TINY_HEAD_BEGIN\s*\{\s*([\s\S]*)\}\s*#TINY_HEAD_END)"))
		{
			auto pack = this->parsePackageHead(head);
			this->splitPasses(pack, passes);
			pack->apply();
		}
	}

	ShaderPackage* ShaderBuilder::parsePackageHead(std::string& content)
	{
		std::unordered_map<std::string, Any> map;
		splitValue(content, map);
		auto pack = new ShaderPackage();
		pack->setConfig(map);
		return pack;
	}

	void ShaderBuilder::splitPasses(ShaderPackage* pack, std::string& content)
	{
		std::regex pattern(R"(#TINY_PASS_BEGIN\s*\{\s*([\s\S]*)\}\s*#TINY_PASS_END)");
		std::sregex_iterator end;
		for (auto i = std::sregex_iterator(content.begin(), content.end(), pattern); i != end; i++)
		{
			std::string temp((*i)[1]);
			auto shader = new Shader();
			this->parseShaders(shader, temp);
			shader->apply();
			pack->addShader(shader);
		}
	}

	void ShaderBuilder::parseShaders(Shader* shader, std::string& content)
	{
		std::string config, shader_content;
		splitConfig(content, config, shader_content, R"(#TINY_CFG_BEGIN\s*\{\s*([\s\S]*)\}\s*#TINY_CFG_END)");

		this->parseShaderConfig(shader, config);
		this->parseShader(shader
			, shader_content
			, R"(#TINY_VS_BEGIN\s*\{\s*([\s\S]*)\}\s*#TINY_VS_END)"
			, GL_VERTEX_SHADER);
		this->parseShader(shader
			, shader_content
			, R"(#TINY_FS_BEGIN\s*\{\s*([\s\S]*)\}\s*#TINY_FS_END)"
			, GL_FRAGMENT_SHADER);
	}

	void ShaderBuilder::parseShaderConfig(Shader* shader, std::string& content)
	{
		std::unordered_map<std::string, Any> map;
		if (splitValue(content, map))
		{
			shader->setVersion(map["Version"].cast<int>());
			shader->setOrderID(map["OrderID"].cast<int>());
			shader->setRenderQueue(Pipeline::getQueue(map["Queue"].cast<std::string>()));
		}
		else
		{
			throw std::logic_error("Shader CFG Error");
		}
	}

	void ShaderBuilder::parseShader(Shader* shader, std::string& content, const char* regex, uint32_t shaderType)
	{
		std::regex regex_shader(regex);
		std::smatch result;
		if (std::regex_search(content, result, regex_shader))
		{
			std::string temp(result[1]);
			temp = "#version " + std::to_string(shader->getVersion()) + " core\n\r" + temp;

			std::regex regex_uniform(R"(uniform\s+(\S+)\s+(\S+)(?=;))");
			std::sregex_iterator end;
			for (auto i = std::sregex_iterator(temp.begin(), temp.end(), regex_uniform); i != end; i++)
			{
// 				std::cout << (*i)[1] << std::endl;
// 				std::cout << (*i)[2] << std::endl;

				shader->registerUniform((*i)[1], (*i)[2]);
			}

			switch (shaderType)
			{
			case GL_VERTEX_SHADER:
				this->loadFromData(shader, temp.c_str(), GL_VERTEX_SHADER);
				break;
			case GL_FRAGMENT_SHADER:
				this->loadFromData(shader, temp.c_str(), GL_FRAGMENT_SHADER);
				break;
			default:
				break;
			}
		}
	}

	void ShaderBuilder::loadFromData(Shader* shader, const char* data, GLenum shaderType)
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
				std::cout << "ERROR::SHADER [VERTEX] COMPILATION_FAILED" << infoLog << std::endl;
				break;
			case GL_FRAGMENT_SHADER:
				std::cout << "ERROR::SHADER [FRAGMENT] COMPILATION_FAILED" << infoLog << std::endl;
				break;
			default:
				break;
			}

		}

		m_ShaderIDs.push_back(shader_id);
		shader->attachShader(shader_id);
	}

	void ShaderBuilder::createPackage(const std::string& filePath)
	{
		ShaderBuilder().loadFromFile(filePath.c_str());
	}
}