#include "Shader.h"

#include <glad/glad.h>
#include "Engine.h"
#include "ShaderManager.h"

namespace tezcat::Tiny::Core
{
	Shader::Shader(const std::string& name, int orderID):
		m_ProgramID(glCreateProgram()),
		m_Name(name),
		m_OrderID(orderID)
	{

	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ProgramID);
	}

	Shader* Shader::create()
	{
		if (m_ProgramID != 0)
		{
			return nullptr;
		}

		return this;
	}

	void Shader::bind()
	{
		glUseProgram(m_ProgramID);
	}

	Shader* Shader::attachShader(ShaderBuilder *shader)
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

		m_ProjectionMatrixID = glGetUniformLocation(m_ProgramID, "PMatrix");
		m_ViewMatrixID = glGetUniformLocation(m_ProgramID, "VMatrix");
		m_ModelMatrixID = glGetUniformLocation(m_ProgramID, "MMatrix");

		Engine::getInstance()->getShaderManager()->addShader(this);
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
}

