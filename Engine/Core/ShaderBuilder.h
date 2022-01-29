#pragma once
#include "CommonHead.h"
#include "GLHead.h"

namespace tezcat::Tiny::Core
{
	/*
	* Shader format
	*
	* TINY_BEGIN_DEF
	* {
	*	Name = Texture;
	*	OrderID = 0;
	* }
	* TINY_END_DEF
	*
	* TINY_BEGIN_VS
	* {
	* }
	* TINY_END_VS
	*
	* TINY_BEGIN_FS
	* {
	* }
	* TINY_END_FS
	*/

	class Shader;
	class ShaderBuilder
	{
	private:
		ShaderBuilder();
		~ShaderBuilder();

	private:
		Shader* loadFromFile(const char* filePath);
		void loadFromData(Shader* shader, const char* data, GLenum shaderType);

		void parse(Shader* shader, std::string& content);
		void parseDefine(Shader* shader, std::string& content);
		void parseShader(Shader* shader, std::string& content, const char* tinyBegin, const char* tinyEnd);

	public:
		static void create(const std::string& filePath);

	private:
		std::vector<unsigned int> m_ShaderIDs;

	private:
		static std::regex TINY_SHADER_FILTER;
		static std::string TINY_SHADER_FILTER_NULL;

		static const char* TINY_BEGIN_DEF;
		static const char* TINY_END_DEF;
		static const char* TINY_BEGIN_VS;
		static const char* TINY_END_VS;
		static const char* TINY_BEGIN_FS;
		static const char* TINY_END_FS;
	};
}

