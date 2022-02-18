#pragma once
#include "Core/Head/CppHead.h"
#include "Core/Head/ConfigHead.h"
#include "Core/Shader/Uniform.h"

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

	class GLShader;
	class ShaderPackage;
	class TINY_API GLShaderBuilder
	{
	private:
		GLShaderBuilder();
		~GLShaderBuilder();

	private:
		GLShader* loadFromFile(const char* filePath);
		void loadFromData(GLShader* shader, const char* data, uint32_t shaderType);

		void parseShaders(GLShader* shader, std::string& content, UniformID::USet& uniformArray);
		void parseShaderConfig(GLShader* shader, std::string& content);
		void parseShader(GLShader* shader, std::string& content, const char* regex, uint32_t shaderType, UniformID::USet& uniformArray);
		void splitPackage(std::string& content);
		void splitPasses(ShaderPackage* pack, std::string& content);
		ShaderPackage* parsePackageHead(std::string& content);

	public:
		static void createPackage(const std::string& filePath);
	private:
		std::vector<uint32_t> m_ShaderIDs;
	};
}

