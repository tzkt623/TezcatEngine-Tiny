#include "Shader.h"
#include "../Engine.h"
#include "../Head/GLHead.h"
#include "../Manager/PipelineManager.h"
#include "../Renderer/Texture.h"

#include "Utility/Tools.h"

namespace tezcat::Tiny::Core
{
	Shader::Shader()
		: Shader("##ErrorShader", 0)
	{

	}

	Shader::Shader(const std::string& name, int orderID)
		: m_ProgramID(glCreateProgram())
		, m_Name(name)
		, m_UID(IDGenerator<Shader, unsigned int>::generate())
		, m_OrderID(orderID)
		, m_ViewMatrixID(0)
		, m_ModelMatrixID(0)
		, m_ProjectionMatrixID(0)
		, m_NormalMatrixID(0)
		, m_ViewPositionID(0)
		, m_EnableBlend(false)
		, m_EnableCullFace(true)
		, m_EnableZWrite(true)
		, m_EnableLighting(true)
		, m_Version(-1)
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

	void Shader::setGPUOptions()
	{
		if (m_EnableCullFace)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}

		if (m_EnableZWrite)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}

		if (m_EnableBlend)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ZERO);
		}
	}

	void Shader::attachShader(uint32_t id)
	{
		glAttachShader(m_ProgramID, id);
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

		m_ProjectionMatrixID = glGetUniformLocation(m_ProgramID, TINY_MatP);
		m_ViewMatrixID = glGetUniformLocation(m_ProgramID, TINY_MatV);
		m_ModelMatrixID = glGetUniformLocation(m_ProgramID, TINY_MatM);
		m_NormalMatrixID = glGetUniformLocation(m_ProgramID, TINY_MatN);
		m_ViewPositionID = glGetUniformLocation(m_ProgramID, TINY_ViewPosition);

		glUseProgram(m_ProgramID);
		if (!m_TextureID.empty())
		{
			int index = 0;
			for (auto& pair : m_TextureID)
			{
				auto id = glGetUniformLocation(m_ProgramID, pair.first.c_str());
				glUniform1i(id, index);
				pair.second = index;
				index++;
			}
		}
		glUseProgram(0);

		PipelineManager::createPass(this);
	}

	void Shader::registerTextureName(const std::string& textureName)
	{
		m_TextureID[textureName] = -1;
	}

	void Shader::registerUniformName(const std::string& uniformName)
	{
		m_UniformID[uniformName] = -1;
	}

	void Shader::registerUniform(const std::string& uniformType, const std::string& uniformName)
	{
		if (uniformType == "sampler2D")
		{
			this->registerTextureName(uniformName);
		}
	}

	void Shader::setTextures(const std::unordered_map<std::string, Texture*>& allTexture)
	{
		if (m_TextureID.empty())
		{
			return;
		}
		for (auto& pair : m_TextureID)
		{
			auto it = allTexture.find(pair.first);
			if (it != allTexture.end())
			{
				glActiveTexture(GL_TEXTURE0 + pair.second);
				glBindTexture(GL_TEXTURE_2D, it->second->getTexID());
			}
		}
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

	void Shader::setNormalMatrix(const glm::mat4x4& matrix)
	{
		if (m_NormalMatrixID > -1)
		{
			auto mat = glm::mat3(glm::transpose(glm::inverse(matrix)));
			glUniformMatrix3fv(m_NormalMatrixID, 1, GL_FALSE, glm::value_ptr(mat));
		}
	}

	void Shader::setViewPosition(const glm::vec3& position)
	{
		if (m_ViewPositionID > -1)
		{
			glUniform3fv(m_ViewPositionID, 1, glm::value_ptr(position));
		}
	}

	void Shader::setFloat1(const std::string& name, float* data)
	{
		glUniform1fv(m_UniformID[name], 1, data);
	}

	void Shader::setFloat2(const std::string& name, float* data)
	{
		glUniform2fv(m_UniformID[name], 1, data);
	}

	void Shader::setFloat3(const std::string& name, float* data)
	{
		glUniform3fv(m_UniformID[name], 1, data);
	}
}

