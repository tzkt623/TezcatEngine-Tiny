#include "GLShader.h"
#include "Core/Renderer/Texture.h"
#include "Core/Tool/Log.h"


namespace tezcat::Tiny::GL
{
	GLShader::GLShader()
		: mLocalTexure(0)
		, mGlobalTexture(0)
	{
	}

	GLShader::~GLShader()
	{
		glDeleteProgram(mProgramID);
	}

	void GLShader::linkShader(uint32_t id)
	{
		glAttachShader(mProgramID, id);
		glDeleteShader(id);
	}

	void GLShader::bind()
	{
		glUseProgram(mProgramID);
	}

	void GLShader::unbind()
	{
		glUseProgram(0);
	}

	void GLShader::buildWithUniforms(const UniformID::USet& uniforms)
	{
		glLinkProgram(mProgramID);

		int success;
		char infoLog[512];
		glGetProgramiv(mProgramID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(mProgramID, 512, nullptr, infoLog);
		}

		mTinyUniformList.resize(UniformID::allStringCount(), -1);
		for (auto& uniform_id : uniforms)
		{
			if (uniform_id.getUID() < mTinyUniformList.size())
			{
				mTinyUniformList[uniform_id.getUID()] = glGetUniformLocation(mProgramID, uniform_id.getStringData());
			}
			else
			{
				Log_Error(StringTool::stringFormat("Your Shader`s buildin value name[%s] write error!!!", uniform_id.getStringData()));
			}
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

	void GLShader::resetLocalState()
	{
		mLocalTexure = 0;
	}

	void GLShader::resetGlobalState()
	{
		mGlobalTexture = 0;
	}

	void GLShader::setFloat1(const char* name, float* data)
	{
		auto id = glGetUniformLocation(mProgramID, name);
		glUniform1fv(id, 1, data);
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

	void GLShader::setFloat2(UniformID& uniform, const glm::vec2& data)
	{
		if (mTinyUniformList[uniform] < 0)
		{
			return;
		}

		glUniform2f(mTinyUniformList[uniform], data.x, data.y);
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

	void GLShader::setInt1(UniformID& uniform, const int& data)
	{
		if (mTinyUniformList[uniform] < 0)
		{
			return;
		}
		glUniform1i(mTinyUniformList[uniform], data);
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

	void GLShader::setMat3(UniformID& uniform, const glm::mat3& mat3)
	{
		if (mTinyUniformList[uniform] < 0)
		{
			return;
		}
		glUniformMatrix3fv(mTinyUniformList[uniform], 1, GL_FALSE, glm::value_ptr(mat3));
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

	void GLShader::setMat4(UniformID& uniform, const glm::mat4& mat4)
	{
		if (mTinyUniformList[uniform] < 0)
		{
			return;
		}

		glUniformMatrix4fv(mTinyUniformList[uniform], 1, GL_FALSE, glm::value_ptr(mat4));
	}

	void GLShader::setMat4(UniformID& uniform, glm::mat4 data[], int count)
	{
		if (mTinyUniformList[uniform] < 0)
		{
			return;
		}

		glUniformMatrix4fv(mTinyUniformList[uniform], count, GL_FALSE, glm::value_ptr(data[0]));
	}

	void GLShader::setGlobalTexture2D(UniformID& uniform, Texture2D* data)
	{
		if (mTinyUniformList[uniform] < 0)
		{
			return;
		}

		glUniform1i(mTinyUniformList[uniform], mGlobalTexture + mLocalTexure);
		glActiveTexture(GL_TEXTURE0 + mGlobalTexture + mLocalTexure);
		glBindTexture(GL_TEXTURE_2D, data->getTextureID());
		++mGlobalTexture;
	}

	void GLShader::setTexture2D(UniformID& uniform, Texture2D* data)
	{
		if (mTinyUniformList[uniform] < 0)
		{
			return;
		}

		glUniform1i(mTinyUniformList[uniform], mGlobalTexture + mLocalTexure);
		glActiveTexture(GL_TEXTURE0 + mGlobalTexture + mLocalTexure);
		glBindTexture(GL_TEXTURE_2D, data->getTextureID());
		++mLocalTexure;
	}

	void GLShader::setTextureCube(UniformID& uniform, TextureCube* data)
	{
		if (mTinyUniformList[uniform] < 0)
		{
			return;
		}

		glUniform1i(mTinyUniformList[uniform], mGlobalTexture + mLocalTexure);
		glActiveTexture(GL_TEXTURE0 + mGlobalTexture + mLocalTexure);
		glBindTexture(GL_TEXTURE_CUBE_MAP, data->getTextureID());
		++mLocalTexure;
	}

	void GLShader::setGlobalTextureCube(UniformID& uniform, TextureCube* data)
	{
		if (mTinyUniformList[uniform] < 0)
		{
			return;
		}

		glUniform1i(mTinyUniformList[uniform], mGlobalTexture + mLocalTexure);
		glActiveTexture(GL_TEXTURE0 + mGlobalTexture + mLocalTexure);
		glBindTexture(GL_TEXTURE_CUBE_MAP, data->getTextureID());
		++mGlobalTexture;
	}

	void GLShader::clear()
	{

	}

	void GLShader::createID()
	{
		if (mProgramID > 0)
		{
			glDeleteProgram(mProgramID);
		}

		mProgramID = glCreateProgram();
	}

}
