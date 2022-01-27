#pragma once
#include <string>
#include <glad/glad.h>
#include <vector>

namespace tezcat::Tiny::Core
{
	class ShaderBuilder
	{
	public:
		ShaderBuilder();
		~ShaderBuilder();

	public:
		void loadFromFile(const char* filePath, GLenum shaderType);
		void loadFromData(const char* data, GLenum shaderType);

		GLuint getID() const { return m_ShaderID; }

	public:
		static ShaderBuilder* createFromPool(const std::string& filePath, GLenum shaderType);
		static void clearPool();

	private:
		GLuint m_ShaderID;

	private:
		static std::vector<ShaderBuilder> Pool;
	};
}

