#pragma once
#include "Core/Shader/Shader.h"
#include "Core/Head/ConfigHead.h"
#include "GLHead.h"


namespace tezcat::Tiny::GL
{
	class TINY_API GLShader : public Shader
	{
	public:
		GLShader();
		~GLShader();

		void attachShader(uint32_t id);

	public:
		void bind() override;
		void unbind() override;
		void setStateOptions() override;
		void resetState() override;

	public:
		void setProjectionMatrix(const glm::mat4x4& matrix) override;
		void setViewMatrix(const glm::mat4x4& matrix) override;
		void setModelMatrix(const glm::mat4x4& matrix) override;
		void setViewPosition(const glm::vec3& position) override;

	public:
		void setFloat1(const char* name, float* data) override;
		void setFloat2(const char* name, float* data) override;
		void setFloat3(const char* name, float* data) override;
		void setFloat4(const char* name, float* data) override;

		void setInt1(const char* name, int* data) override;
		void setInt2(const char* name, int* data) override;
		void setInt3(const char* name, int* data) override;
		void setInt4(const char* name, int* data) override;

		void setMat3(const char* name, float* data) override;
		void setMat4(const char* name, const float* data) override;
		void setMat4(UniformID& uniform, const glm::mat4& mat4) override;

	public:

		void setFloat1(UniformID& uniform, float* data) override;
		void setFloat2(UniformID& uniform, float* data) override;
		void setFloat2(UniformID& uniform, const glm::vec2& data) override;
		void setFloat3(UniformID& uniform, float* data) override;
		void setFloat4(UniformID& uniform, float* data) override;

		void setInt1(UniformID& uniform, const int& data) override;
		void setInt1(UniformID& uniform, int* data) override;
		void setInt2(UniformID& uniform, int* data) override;
		void setInt3(UniformID& uniform, int* data) override;
		void setInt4(UniformID& uniform, int* data) override;
		void setMat3(UniformID& uniform, float* data) override;
		void setMat3(UniformID& uniform, const glm::mat3& mat3) override;
		void setMat4(UniformID& uniform, const float* data) override;
		void setTexture2D(UniformID& uniform, Texture2D* data) override;
		void setTextureCube(UniformID& uniform, TextureCube* data) override;

	public:
		void create() override;
		void clear() override;


	protected:
		void onApply(const UniformID::USet& uniforms) override;

	private:
		std::unordered_map<std::string, uint32_t> mTextureID;
		std::unordered_map<std::string, uint32_t> mUniformID;
		int mTexureCountor;
	};
}
