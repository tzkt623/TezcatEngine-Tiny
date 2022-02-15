#include "GLShaderBuilder.h"
#include "GLShader.h"
#include "GLHead.h"
#include "Core/Manager/ShaderManager.h"
#include "Core/Shader/ShaderPackage.h"


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
					else if (value_pair_result[3] == "false")
					{
						map.emplace(value_pair_result[2], false);
					}
					else
					{
						throw std::logic_error("GLShader: Shader Param [bool]`s string must be [true] or [false]");
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
		m_ShaderIDs.reserve(4);
	}

	GLShaderBuilder::~GLShaderBuilder()
	{
		for (auto id : m_ShaderIDs)
		{
			std::cout << "ShaderBuilder : " << id << "Deleted" << std::endl;
			glDeleteShader(id);
		}
		m_ShaderIDs.clear();
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

	void GLShaderBuilder::splitPackage(std::string& content)
	{
		std::string head, passes;
		if (splitConfig(content, head, passes, R"(#TINY_HEAD_BEGIN\s*\{\s*([\s\S]*)\}\s*#TINY_HEAD_END)"))
		{
			auto pack = this->parsePackageHead(head);
			this->splitPasses(pack, passes);
			pack->apply();
		}
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
		std::regex pattern(R"(#TINY_PASS_BEGIN\s*\{\s*([\s\S]*)\}\s*#TINY_PASS_END)");
		std::sregex_iterator end;
		for (auto i = std::sregex_iterator(content.begin(), content.end(), pattern); i != end; i++)
		{
			std::string temp((*i)[1]);
			auto shader = new GLShader();
			this->parseShaders(shader, temp);
			shader->apply();
			pack->addShader(shader);
		}
	}

	void GLShaderBuilder::parseShaders(GLShader* shader, std::string& content)
	{
		std::string config, shader_content;
		if (splitConfig(content, config, shader_content, R"(#TINY_CFG_BEGIN\s*\{\s*([\s\S]*)\}\s*#TINY_CFG_END)"))
		{
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

			//OrderID
			auto it = map.find("OrderID");
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
				shader->setRenderQueue(Pipeline::getQueue(it->second.cast<std::string>()));
			}
			else
			{
				shader->setRenderQueue(Pipeline::Queue::Opaque);
			}

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

			//Blend
			it = map.find("Blend");
			if (it != map.end() && it->second.cast<bool>())
			{
				shader->setBlend(true);
				shader->setBlendFunction(GLShader::blendMap[map["BlendSrc"].cast<std::string>()]
					, GLShader::blendMap[map["BlendTar"].cast<std::string>()]);
			}
			else
			{
				shader->setBlend(false);
			}

			//CullFace
			it = map.find("CullFace");
			if (it != map.end())
			{
				shader->setCullFace(GLShader::cullFaceMap[it->second.cast<std::string>()]);
			}
			else
			{
				shader->setCullFace(0);
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

	void GLShaderBuilder::parseShader(GLShader* shader, std::string& content, const char* regex, uint32_t shaderType)
	{
		std::regex regex_shader(regex);
		std::smatch result;
		if (std::regex_search(content, result, regex_shader))
		{
			std::string temp(result[1]);
			std::regex regex_uniform(R"(uniform\s+(\w+)\s+(\w+)(?=[\s\S]*;))");
			std::sregex_iterator end;
			for (auto i = std::sregex_iterator(temp.begin(), temp.end(), regex_uniform); i != end; i++)
			{
				// 				std::cout << (*i)[1] << std::endl;
				// 				std::cout << (*i)[2] << std::endl;

				shader->registerUniform((*i)[1], (*i)[2]);
			}

			temp = "#version " + std::to_string(shader->getVersion()) + " core\n\r" + temp;
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
				std::cout << "GLShader: [VERTEX] COMPILATION_FAILED > " << infoLog << std::endl;
				break;
			case GL_FRAGMENT_SHADER:
				std::cout << "GLShader: [FRAGMENT] COMPILATION_FAILED > " << infoLog << std::endl;
				break;
			default:
				break;
			}
		}

		m_ShaderIDs.push_back(shader_id);
		shader->attachShader(shader_id);
	}

	void GLShaderBuilder::createPackage(const std::string& filePath)
	{
		TINY_PROFILER_TIMER_FUNCTION();
		GLShaderBuilder().loadFromFile(filePath.c_str());
		TINY_PROFILER_TIMER_FUNCTION_LOG();
	}
}