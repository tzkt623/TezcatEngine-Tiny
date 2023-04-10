#include "GLShader.h"
#include "Core/Renderer/Texture.h"


namespace tezcat::Tiny::GL
{
	GLShader::GLShader()
		: mTexureCountor(0)
	{
		mProgramID = glCreateProgram();
	}

	GLShader::~GLShader()
	{
		glDeleteProgram(mProgramID);
	}

	void GLShader::attachShader(uint32_t id)
	{
		glAttachShader(mProgramID, id);
	}

	void GLShader::bind()
	{
		glUseProgram(mProgramID);
	}

	void GLShader::unbind()
	{
		glUseProgram(0);
	}

	void GLShader::onApply(const UniformID::USet& uniforms)
	{
		glLinkProgram(mProgramID);

		int success;
		char infoLog[512];
		glGetProgramiv(mProgramID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(mProgramID, 512, nullptr, infoLog);
		}

		for (auto& uniform_id : uniforms)
		{
			mTinyUniformList[uniform_id.getUID()] = glGetUniformLocation(mProgramID, uniform_id.getStringData());
		}

		glUseProgram(0);
	}

	void GLShader::setStateOptions()
	{
		if (mCullFace.platform != 0)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(mCullFace.platform);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}

		if (mDepthTest.platform != 0)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(mDepthTest.platform);

		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}

		//只有在深度测试启用的情况下才有用
		glDepthMask(mEnableZWrite ? GL_TRUE : GL_FALSE);

		if (mEnableBlend)
		{
			glEnable(GL_BLEND);
			glBlendFunc(mBlendSource.platform, mBlendTarget.platform);
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}

	void GLShader::setProjectionMatrix(const glm::mat4x4& matrix)
	{
		glUniformMatrix4fv(mTinyUniformList[ShaderParam::MatrixP], 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void GLShader::setViewMatrix(const glm::mat4x4& matrix)
	{
		glUniformMatrix4fv(mTinyUniformList[ShaderParam::MatrixV], 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void GLShader::setModelMatrix(const glm::mat4x4& matrix)
	{
		glUniformMatrix4fv(mTinyUniformList[ShaderParam::MatrixM], 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void GLShader::setViewPosition(const glm::vec3& position)
	{
		if (mTinyUniformList[ShaderParam::ViewPosition] > -1)
		{
			glUniform3fv(mTinyUniformList[ShaderParam::ViewPosition], 1, glm::value_ptr(position));
		}
	}

	void GLShader::setFloat1(const char* name, float* data)
	{
		glUniform1fv(glGetUniformLocation(mProgramID, name), 1, data);
	}

	void GLShader::setFloat1(UniformID& uniform, float* data)
	{
		if (mTinyUniformList[uniform] < 0)
		{
			return;
		}
		glUniform1fv(mTinyUniformList[uniform], 1, data);
	}

	void GLShader::setFloat2(const char* name, float* data)
	{
		glUniform2fv(glGetUniformLocation(mProgramID, name), 1, data);
	}

	void GLShader::setFloat2(UniformID& uniform, float* data)
	{
		if (mTinyUniformList[uniform] < 0)
		{
			return;
		}
		glUniform2fv(mTinyUniformList[uniform], 1, data);
	}

	void GLShader::setFloat3(const char* name, float* data)
	{
		glUniform3fv(glGetUniformLocation(mProgramID, name), 1, data);
	}

	void GLShader::setFloat3(UniformID& uniform, float* data)
	{
		if (mTinyUniformList[uniform] < 0)
		{
			return;
		}
		glUniform3fv(mTinyUniformList[uniform], 1, data);
	}

	void GLShader::setFloat4(UniformID& uniform, float* data)
	{
		if (mTinyUniformList[uniform] < 0)
		{
			return;
		}
		glUniform4fv(mTinyUniformList[uniform], 1, data);
	}

	void GLShader::setFloat4(const char* name, float* data)
	{
		glUniform4fv(glGetUniformLocation(mProgramID, name), 1, data);
	}

	void GLShader::setInt1(UniformID& uniform, int* data)
	{
		if (mTinyUniformList[uniform] < 0)
		{
			return;
		}
		glUniform1iv(mTinyUniformList[uniform], 1, data);
	}

	void GLShader::setInt1(const char* name, int* data)
	{
		glUniform1iv(glGetUniformLocation(mProgramID, name), 1, data);
	}

	void GLShader::setInt2(UniformID& uniform, int* data)
	{
		if (mTinyUniformList[uniform] < 0)
		{
			return;
		}
		glUniform2iv(mTinyUniformList[uniform], 1, data);
	}

	void GLShader::setInt2(const char* name, int* data)
	{
		glUniform2iv(glGetUniformLocation(mProgramID, name), 1, data);
	}

	void GLShader::setInt3(UniformID& uniform, int* data)
	{
		if (mTinyUniformList[uniform] < 0)
		{
			return;
		}
		glUniform3iv(mTinyUniformList[uniform], 1, data);
	}

	void GLShader::setInt3(const char* name, int* data)
	{
		glUniform3iv(glGetUniformLocation(mProgramID, name), 1, data);
	}

	void GLShader::setInt4(UniformID& uniform, int* data)
	{
		if (mTinyUniformList[uniform] < 0)
		{
			return;
		}
		glUniform4iv(mTinyUniformList[uniform], 1, data);
	}

	void GLShader::setInt4(const char* name, int* data)
	{
		glUniform4iv(glGetUniformLocation(mProgramID, name), 1, data);
	}

	void GLShader::setMat3(UniformID& uniform, float* data)
	{
		if (mTinyUniformList[uniform] < 0)
		{
			return;
		}
		glUniformMatrix3fv(mTinyUniformList[uniform], 1, GL_FALSE, data);
	}

	void GLShader::setMat3(const char* name, float* data)
	{
		glUniformMatrix3fv(glGetUniformLocation(mProgramID, name), 1, GL_FALSE, data);
	}

	void GLShader::setMat4(UniformID& uniform, const float* data)
	{
		if (mTinyUniformList[uniform] < 0)
		{
			return;
		}
		glUniformMatrix4fv(mTinyUniformList[uniform], 1, GL_FALSE, data);
	}

	void GLShader::setMat4(const char* name, const float* data)
	{
		glUniformMatrix4fv(glGetUniformLocation(mProgramID, name), 1, GL_FALSE, data);
	}

	void GLShader::setTexture2D(UniformID& uniform, Texture2D* data)
	{
		if (mTinyUniformList[uniform] < 0)
		{
			return;
		}
		glUniform1i(mTinyUniformList[uniform], mTexureCountor);
		glActiveTexture(GL_TEXTURE0 + mTexureCountor++);
		glBindTexture(GL_TEXTURE_2D, data->getTextureID());
	}

	void GLShader::setTextureCube(UniformID& uniform, TextureCube* data)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, data->getTextureID());
	}

	void GLShader::resetState()
	{
		mTexureCountor = 0;
	}
}
