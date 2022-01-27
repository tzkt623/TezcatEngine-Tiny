#include "ShaderBuilder.h"
#include <iostream>
#include <fstream>
#include <sstream>


namespace tezcat::Tiny::Core
{
	std::vector<ShaderBuilder> ShaderBuilder::Pool;

	ShaderBuilder::ShaderBuilder() :
		m_ShaderID(-1)
	{

	}

	ShaderBuilder::~ShaderBuilder()
	{
		std::cout << "ShaderBuilder : " << m_ShaderID << "Deleted" << std::endl;
		glDeleteShader(m_ShaderID);
		m_ShaderID = -1;
	}

	void ShaderBuilder::loadFromFile(const char* filePath, GLenum shaderType)
	{
		std::fstream io(filePath, std::ios::in | std::ios::binary);
		if (io.is_open())
		{
			std::string data;
			std::stringstream buf;
			buf << io.rdbuf();
			data = buf.str();

			this->loadFromData(data.c_str(), shaderType);
		}
		io.close();
	}

	void ShaderBuilder::loadFromData(const char* data, GLenum shaderType)
	{
		m_ShaderID = glCreateShader(shaderType);
		glShaderSource(m_ShaderID, 1, &data, nullptr);
		glCompileShader(m_ShaderID);

		int success;
		char infoLog[512];
		glGetShaderiv(m_ShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(m_ShaderID, 512, nullptr, infoLog);
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
	}

	void ShaderBuilder::clearPool()
	{
		ShaderBuilder::Pool.clear();
	}

	ShaderBuilder* ShaderBuilder::createFromPool(const std::string& filePath, GLenum shaderType)
	{
		auto p = &ShaderBuilder::Pool.emplace_back();
		p->loadFromFile(filePath.c_str(), shaderType);
		return p;
	}


}