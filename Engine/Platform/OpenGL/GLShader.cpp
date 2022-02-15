#include "GLShader.h"
#include "Core/Renderer/Texture.h"


namespace tezcat::Tiny::Core
{
	GLShader::GLShader()
		: m_CullFace(GL_BACK)
		, m_BlendSource(GL_ONE)
		, m_BlendTarget(GL_ONE_MINUS_SRC_ALPHA)
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
		if (m_CullFace > 0)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(m_CullFace);
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
			glBlendFunc(m_BlendSource, m_BlendTarget);
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}

	void GLShader::setProjectionMatrix(const glm::mat4x4& matrix)
	{
		glUniformMatrix4fv(m_ProjectionMatrixID, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void GLShader::setViewMatrix(const glm::mat4x4& matrix)
	{
		glUniformMatrix4fv(m_ViewMatrixID, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void GLShader::setModelMatrix(const glm::mat4x4& matrix)
	{
		glUniformMatrix4fv(m_ModelMatrixID, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void GLShader::setViewPosition(const glm::vec3& position)
	{
		if (m_ViewPositionID > -1)
		{
			glUniform3fv(m_ViewPositionID, 1, glm::value_ptr(position));
		}
	}

	void GLShader::setNormalMatrix(const glm::mat4x4& matrix)
	{
		if (m_NormalMatrixID > -1)
		{
			auto mat = glm::mat3(glm::transpose(glm::inverse(matrix)));
			glUniformMatrix3fv(m_NormalMatrixID, 1, GL_FALSE, glm::value_ptr(mat));
		}
	}

	void GLShader::setTextures(const std::unordered_map<std::string, Texture*>& allTexture)
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

	void GLShader::setFloat1(const std::string& name, float* data)
	{
		glUniform1fv(m_UniformID[name], 1, data);
	}

	void GLShader::setFloat2(const std::string& name, float* data)
	{
		glUniform2fv(m_UniformID[name], 1, data);
	}

	void GLShader::setFloat3(const std::string& name, float* data)
	{
		glUniform3fv(m_UniformID[name], 1, data);
	}

	void GLShader::bind()
	{
		glUseProgram(m_ProgramID);
	}

	void GLShader::onApply()
	{
		glLinkProgram(m_ProgramID);

		int success;
		char infoLog[512];
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(m_ProgramID, 512, nullptr, infoLog);
		}

		m_ProjectionMatrixID = glGetUniformLocation(m_ProgramID, TinyParameter::MatP);
		m_ViewMatrixID = glGetUniformLocation(m_ProgramID, TinyParameter::MatV);
		m_ModelMatrixID = glGetUniformLocation(m_ProgramID, TinyParameter::MatM);
		m_NormalMatrixID = glGetUniformLocation(m_ProgramID, TinyParameter::MatN);
		m_ViewPositionID = glGetUniformLocation(m_ProgramID, TinyParameter::ViewPosition);

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
	}

	void GLShader::registerTextureName(const std::string& textureName)
	{
		m_TextureID[textureName] = -1;
	}

	void GLShader::registerUniformName(const std::string& uniformName)
	{
		m_UniformID[uniformName] = -1;
	}

	void GLShader::registerUniform(const std::string& uniformType, const std::string& uniformName)
	{
		if (uniformType == "sampler2D")
		{
			this->registerTextureName(uniformName);
		}
	}

	void GLShader::setCullFace(int value)
	{
		m_CullFace = value;
	}

	void GLShader::setBlendFunction(int source, int target)
	{
		m_BlendSource = source;
		m_BlendTarget = target;
	}

	std::unordered_map<std::string, int> GLShader::blendMap =
	{
		{"0",			GL_ZERO},
		{"1",			GL_ONE},
		{"Src",			GL_SRC_COLOR},
		{"1-Src",		GL_ONE_MINUS_SRC_COLOR},
		{"Tar",			GL_DST_COLOR},
		{"1-Tar",		GL_ONE_MINUS_DST_COLOR},
		{"SrcA",		GL_SRC_ALPHA},
		{"1-SrcA",		GL_ONE_MINUS_SRC_ALPHA},
		{"TarA",		GL_DST_ALPHA},
		{"1-TarA",		GL_ONE_MINUS_DST_ALPHA},
		{"Const",		GL_CONSTANT_COLOR},
		{"1-Const",		GL_ONE_MINUS_CONSTANT_COLOR},
		{"ConstA",		GL_CONSTANT_ALPHA},
		{"1-ConstA",	GL_ONE_MINUS_CONSTANT_ALPHA}
	};

	std::unordered_map<std::string, int> GLShader::cullFaceMap =
	{
		{"Off",		0},
		{"Front",	GL_FRONT},
		{"Back",	GL_BACK},
		{"All",		GL_FRONT_AND_BACK}
	};

}
