#include "GLShader.h"
#include "Core/Renderer/Texture.h"


namespace tezcat::Tiny::Core
{
	GLShader::GLShader()
		: m_TexureCountor(0)
	{
		m_ProgramID = glCreateProgram();
	}

	GLShader::~GLShader()
	{
		glDeleteProgram(m_ProgramID);
	}

	void GLShader::attachShader(uint32_t id)
	{
		glAttachShader(m_ProgramID, id);
	}

	void GLShader::setGPUOptions()
	{
		if (m_CullFace.platform != 0)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(m_CullFace.platform);
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
			glBlendFunc(m_BlendSource.platform, m_BlendTarget.platform);
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}

	void GLShader::setProjectionMatrix(const glm::mat4x4& matrix)
	{
		glUniformMatrix4fv(m_TinyUniformList[ShaderParam::MatrixP], 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void GLShader::setViewMatrix(const glm::mat4x4& matrix)
	{
		glUniformMatrix4fv(m_TinyUniformList[ShaderParam::MatrixV], 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void GLShader::setModelMatrix(const glm::mat4x4& matrix)
	{
		glUniformMatrix4fv(m_TinyUniformList[ShaderParam::MatrixM], 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void GLShader::setViewPosition(const glm::vec3& position)
	{
		if (m_TinyUniformList[ShaderParam::ViewPosition] > -1)
		{
			glUniform3fv(m_TinyUniformList[ShaderParam::ViewPosition], 1, glm::value_ptr(position));
		}
	}

	void GLShader::setNormalMatrix(const glm::mat4x4& matrix)
	{
		if (m_TinyUniformList[ShaderParam::MatrixN] > -1)
		{
			auto mat = glm::mat3(glm::transpose(glm::inverse(matrix)));
			glUniformMatrix3fv(m_TinyUniformList[ShaderParam::MatrixN], 1, GL_FALSE, glm::value_ptr(mat));
		}
	}

	void GLShader::setTextures(const UniformID::UMap<Texture*>& allTexture)
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
				glBindTexture(GL_TEXTURE_2D, it->second->getTextureID());
			}
		}
	}

	void GLShader::bind()
	{
		glUseProgram(m_ProgramID);
	}

	void GLShader::onApply(const UniformID::USet& uniforms)
	{
		glLinkProgram(m_ProgramID);

		int success;
		char infoLog[512];
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(m_ProgramID, 512, nullptr, infoLog);
		}

		for (auto& uniform_id : uniforms)
		{
			m_TinyUniformList[uniform_id.getUID()] = glGetUniformLocation(m_ProgramID, uniform_id.getStringData());
		}

		glUseProgram(0);
	}

	void GLShader::setFloat1(const char* name, float* data)
	{
		glUniform1fv(glGetUniformLocation(m_ProgramID, name), 1, data);
	}

	void GLShader::setFloat1(UniformID& uniform, float* data)
	{
		if (m_TinyUniformList[uniform] < 0)
		{
			return;
		}
		glUniform1fv(m_TinyUniformList[uniform], 1, data);
	}

	void GLShader::setFloat2(const char* name, float* data)
	{
		glUniform2fv(glGetUniformLocation(m_ProgramID, name), 1, data);
	}

	void GLShader::setFloat2(UniformID& uniform, float* data)
	{
		if (m_TinyUniformList[uniform] < 0)
		{
			return;
		}
		glUniform2fv(m_TinyUniformList[uniform], 1, data);
	}

	void GLShader::setFloat3(const char* name, float* data)
	{
		glUniform3fv(glGetUniformLocation(m_ProgramID, name), 1, data);
	}

	void GLShader::setFloat3(UniformID& uniform, float* data)
	{
		if (m_TinyUniformList[uniform] < 0)
		{
			return;
		}
		glUniform3fv(m_TinyUniformList[uniform], 1, data);
	}

	void GLShader::setFloat4(UniformID& uniform, float* data)
	{
		if (m_TinyUniformList[uniform] < 0)
		{
			return;
		}
		glUniform4fv(m_TinyUniformList[uniform], 1, data);
	}

	void GLShader::setFloat4(const char* name, float* data)
	{
		glUniform4fv(glGetUniformLocation(m_ProgramID, name), 1, data);
	}

	void GLShader::setInt1(UniformID& uniform, int* data)
	{
		if (m_TinyUniformList[uniform] < 0)
		{
			return;
		}
		glUniform1iv(m_TinyUniformList[uniform], 1, data);
	}

	void GLShader::setInt1(const char* name, int* data)
	{
		glUniform1iv(glGetUniformLocation(m_ProgramID, name), 1, data);
	}

	void GLShader::setInt2(UniformID& uniform, int* data)
	{
		if (m_TinyUniformList[uniform] < 0)
		{
			return;
		}
		glUniform2iv(m_TinyUniformList[uniform], 1, data);
	}

	void GLShader::setInt2(const char* name, int* data)
	{
		glUniform2iv(glGetUniformLocation(m_ProgramID, name), 1, data);
	}

	void GLShader::setInt3(UniformID& uniform, int* data)
	{
		if (m_TinyUniformList[uniform] < 0)
		{
			return;
		}
		glUniform3iv(m_TinyUniformList[uniform], 1, data);
	}

	void GLShader::setInt3(const char* name, int* data)
	{
		glUniform3iv(glGetUniformLocation(m_ProgramID, name), 1, data);
	}

	void GLShader::setInt4(UniformID& uniform, int* data)
	{
		if (m_TinyUniformList[uniform] < 0)
		{
			return;
		}
		glUniform4iv(m_TinyUniformList[uniform], 1, data);
	}

	void GLShader::setInt4(const char* name, int* data)
	{
		glUniform4iv(glGetUniformLocation(m_ProgramID, name), 1, data);
	}

	void GLShader::setMat3(UniformID& uniform, float* data)
	{
		if (m_TinyUniformList[uniform] < 0)
		{
			return;
		}
		glUniformMatrix3fv(m_TinyUniformList[uniform], 1, GL_FALSE, data);
	}

	void GLShader::setMat3(const char* name, float* data)
	{
		glUniformMatrix3fv(glGetUniformLocation(m_ProgramID, name), 1, GL_FALSE, data);
	}

	void GLShader::setMat4(UniformID& uniform, float* data)
	{
		if (m_TinyUniformList[uniform] < 0)
		{
			return;
		}
		glUniformMatrix4fv(m_TinyUniformList[uniform], 1, GL_FALSE, data);
	}

	void GLShader::setMat4(const char* name, float* data)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_ProgramID, name), 1, GL_FALSE, data);
	}

	void GLShader::setTexture2D(UniformID& uniform, Texture2D* data)
	{
		if (m_TinyUniformList[uniform] < 0)
		{
			return;
		}
		glUniform1i(m_TinyUniformList[uniform], m_TexureCountor);
		glActiveTexture(GL_TEXTURE0 + m_TexureCountor++);
		glBindTexture(GL_TEXTURE_2D, data->getTextureID());
	}

	void GLShader::resetState()
	{
		m_TexureCountor = 0;
	}

}
