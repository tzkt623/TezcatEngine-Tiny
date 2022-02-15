#pragma once
#include "Core/Shader/Shader.h"
#include "Core/Head/ConfigHead.h"
#include "GLHead.h"


namespace tezcat::Tiny::Core
{
	class TINY_API GLShader : public Shader
	{
	public:
		GLShader();
		~GLShader();

		void attachShader(uint32_t id);

	public:
		void bind() override;
		void registerTextureName(const std::string& textureName) override;
		void registerUniformName(const std::string& uniformName) override;
		void registerUniform(const std::string& uniformType, const std::string& uniformName) override;

	public:
		void setGPUOptions() override;
		void setCullFace(int value) override;
		void setBlendFunction(int source, int target) override;

	public:
		void setProjectionMatrix(const glm::mat4x4& matrix) override;
		void setViewMatrix(const glm::mat4x4& matrix) override;
		void setModelMatrix(const glm::mat4x4& matrix) override;
		void setViewPosition(const glm::vec3& position) override;
		void setNormalMatrix(const glm::mat4x4& matrix) override;
		void setTextures(const std::unordered_map<std::string, Texture*>& allTexture) override;
		void setFloat1(const std::string& name, float* data) override;
		void setFloat2(const std::string& name, float* data) override;
		void setFloat3(const std::string& name, float* data) override;

	protected:
		void onApply() override;

	private:
		std::unordered_map<std::string, uint32_t> m_TextureID;
		std::unordered_map<std::string, uint32_t> m_UniformID;

	private:
		GLenum m_CullFace;
		GLenum m_BlendSource;
		GLenum m_BlendTarget;

	public:
		static std::unordered_map<std::string, int> blendMap;
		static std::unordered_map<std::string, int> cullFaceMap;
	};
}