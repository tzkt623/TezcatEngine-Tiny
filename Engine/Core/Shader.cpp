#include "Shader.h"
#include "Engine.h"
#include "ShaderManager.h"
#include "Texture.h"

namespace tezcat::Tiny::Core
{
	Shader::Shader(const std::string& name, int orderID) :
		m_ProgramID(glCreateProgram()),
		m_Name(name),
		m_OrderID(orderID),
		m_ViewMatrixID(0),
		m_ModelMatrixID(0),
		m_ProjectionMatrixID(0)
	{

	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ProgramID);
	}

	void Shader::bind()
	{
		glUseProgram(m_ProgramID);
	}

	void Shader::setTextures(const std::unordered_map<std::string, Core::Texture*>& allTexture)
	{
		int index = 0;
		for (auto& pair : allTexture)
		{
			glActiveTexture(GL_TEXTURE0 + m_TextureID[pair.first]);
			glBindTexture(GL_TEXTURE_2D, pair.second->getTexID());
		}
	}

	Shader* Shader::attachShader(ShaderBuilder* shader)
	{
		glAttachShader(m_ProgramID, shader->getID());
		return this;
	}

	Shader* Shader::attachShader(const std::string& filePath, GLenum shaderType)
	{
		ShaderBuilder* builder = ShaderBuilder::createFromPool(filePath, shaderType);
		glAttachShader(m_ProgramID, builder->getID());
		return this;
	}

	void Shader::apply()
	{
		glLinkProgram(m_ProgramID);

		int success;
		char infoLog[512];
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(m_ProgramID, 512, nullptr, infoLog);
		}

		ShaderBuilder::clearPool();

		m_ProjectionMatrixID = glGetUniformLocation(m_ProgramID, TINY_MatP);
		m_ViewMatrixID = glGetUniformLocation(m_ProgramID, TINY_MatV);
		m_ModelMatrixID = glGetUniformLocation(m_ProgramID, TINY_MatM);

		glUseProgram(m_ProgramID);
		if (!m_TextureID.empty())
		{
			int index = 0;
			for (auto& pair : m_TextureID)
			{
				auto id = glGetUniformLocation(m_ProgramID, pair.first.c_str());
				glUniform1i(id, index++);
				pair.second = id;
			}
		}
		glUseProgram(0);

		ShaderManager::getInstance()->addShader(this);
	}

	void Shader::registerTextureName(const std::string& textureTypeName)
	{
		m_TextureID[textureTypeName] = -1;
	}

	void Shader::registerUniformName(const std::string& uniformName)
	{
		m_UniformID[uniformName] = -1;
	}

	void Shader::setProjectionMatrix(const glm::mat4x4& matrix)
	{
		glUniformMatrix4fv(m_ProjectionMatrixID, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::setViewMatrix(const glm::mat4x4& matrix)
	{
		glUniformMatrix4fv(m_ViewMatrixID, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::setModelMatrix(const glm::mat4x4& matrix)
	{
		glUniformMatrix4fv(m_ModelMatrixID, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::setFloat1(const std::string& name, float* data)
	{
		glUniform1fv(m_TextureID[name], 1, data);
	}

	void Shader::setFloat2(const std::string& name, float* data)
	{
		glUniform2fv(m_TextureID[name], 2, data);
	}

	void Shader::setFloat3(const std::string& name, float* data)
	{
		glUniform3fv(m_TextureID[name], 3, data);
	}
}

