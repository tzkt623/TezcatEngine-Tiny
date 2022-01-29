#include "ShaderBuilder.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include "Shader.h"


namespace tezcat::Tiny::Core
{
	static std::regex TINY_SHADER_INFO_FILTER(" |\t|\r|\n");
	static std::string TINY_FILTER_FMT_NULL;

	std::regex ShaderBuilder::TINY_SHADER_FILTER(" |\t|\r|\n");

	const char* ShaderBuilder::TINY_BEGIN_DEF = "TINY_BEGIN_DEF";
	const char* ShaderBuilder::TINY_END_DEF = "TINY_END_DEF";

	const char* ShaderBuilder::TINY_BEGIN_VS = "TINY_BEGIN_VS";
	const char* ShaderBuilder::TINY_END_VS = "TINY_END_VS";

	const char* ShaderBuilder::TINY_BEGIN_FS = "TINY_BEGIN_FS";
	const char* ShaderBuilder::TINY_END_FS = "TINY_END_FS";

	bool splitString(const std::string& content, std::string& result1, std::string& result2, const char* spliter)
	{
		auto spliter_length = std::strlen(spliter);
		auto pos = content.find(spliter);
		if (pos != std::string::npos)
		{
			result1 = content.substr(0, pos + spliter_length);
			result2 = content.substr(pos + spliter_length, content.size() - pos - spliter_length);
			return true;
		}

		return false;
	}

	void parsePair(const std::string& content, const char* tokenName, std::string& tokenValue)
	{
		size_t token_begin = 0;
		size_t token_end = 0;
		auto pos = content.find(tokenName);
		for (int i = pos; i < content.size(); i++)
		{
			if (content[i] == '=')
			{
				token_begin = i;
			}
			else if (content[i] == ';')
			{
				token_end = i;
				break;
			}
		}

		tokenValue = content.substr(token_begin + 1, token_end - token_begin - 1);
	}

	bool parseRange(const std::string& content, std::string& newContent, const char* tokenBegin, const char* tokenEnd)
	{
		auto begin = content.find(tokenBegin);
		auto end = content.find(tokenEnd);

		if (begin != std::string::npos && end != std::string::npos)
		{
			auto token_length = std::strlen(tokenBegin) + begin;
			newContent = content.substr(token_length + 1, end - 1 - token_length - 1);
			return true;
		}

		newContent = "##Error##Content##";
		return false;
	}

	bool parseShaderRange(const std::string& content, std::string& newContent, const char* tokenBegin, const char* tokenEnd)
	{
		auto begin = content.find(tokenBegin);
		auto end = content.find(tokenEnd);

		if (begin != std::string::npos && end != std::string::npos)
		{
			int offset = 0;
			auto token_begin_pos = std::strlen(tokenBegin) + begin;
			for (auto i = token_begin_pos; i < content.size(); i++)
			{
				if (content[i] == '{')
				{
					break;
				}
				else
				{
					offset++;
				}
			}
			token_begin_pos += offset;

			offset = 0;
			for (auto i = end; i > token_begin_pos; i--)
			{
				if (content[i] == '}')
				{
					break;
				}
				else
				{
					offset++;
				}
			}

			auto token_end_pos = end - offset;
			newContent = content.substr(token_begin_pos + 1, token_end_pos - 1 - token_begin_pos - 1);
			return true;
		}

		newContent = "##Error##Content##";
		return false;
	}

	//----------------------------------------------------------------------
	//
	//
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

			shader = new Shader();
			this->parse(shader, data);
		}
		io.close();
		return shader;
	}

	void ShaderBuilder::parse(Shader* shader, std::string& content)
	{
		std::string define, shader_content;
		splitString(content, define, shader_content, TINY_END_DEF);

		define = std::regex_replace(define, TINY_SHADER_INFO_FILTER, TINY_FILTER_FMT_NULL);

		this->parseDefine(shader, define);
		this->parseShader(shader, shader_content, TINY_BEGIN_VS, TINY_END_VS);
		this->parseShader(shader, shader_content, TINY_BEGIN_FS, TINY_END_FS);
	}

	void ShaderBuilder::parseDefine(Shader* shader, std::string& content)
	{
		static const char* OrderID = "OrderID";
		static const char* Name = "Name";

		std::string new_content;
		if (parseRange(content, new_content, TINY_BEGIN_DEF, TINY_END_DEF))
		{
			std::string value;

			parsePair(new_content, OrderID, value);
			shader->setOrderID(std::stoi(value));
			parsePair(new_content, Name, value);
			shader->setName(value);
		}
		else
		{
			std::cout << "Can`t find token [" << TINY_BEGIN_DEF << "]" << std::endl;
		}
	}

	void ShaderBuilder::parseShader(Shader* shader, std::string& content, const char* tinyBegin, const char* tinyEnd)
	{
		std::string new_content;
		if (parseShaderRange(content, new_content, tinyBegin, tinyEnd))
		{
			new_content = "#version 330 core\n\r" + new_content;

			if (tinyBegin == TINY_BEGIN_VS)
			{
				this->loadFromData(shader, new_content.c_str(), GL_VERTEX_SHADER);
			}
			else if (tinyBegin = TINY_BEGIN_FS)
			{
				this->loadFromData(shader, new_content.c_str(), GL_FRAGMENT_SHADER);
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

	void ShaderBuilder::create(const std::string& filePath)
	{
		ShaderBuilder builder;
		auto shader = builder.loadFromFile(filePath.c_str());
		shader->apply();
	}
}