#pragma once
#include "Core/Head/CppHead.h"
#include "Core/Head/ConfigHead.h"
#include "Core/Shader/Uniform.h"
#include "Core/Manager/ShaderManager.h"

namespace tezcat::Tiny::GL
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
	class TINY_API GLShaderBuilder
	{
	public:
		GLShaderBuilder();
		~GLShaderBuilder();
		GLShader* loadFromFile(const char* filePath);
		ShaderPackage* splitPackage(std::string& content);

	private:
		void loadFromData(GLShader* shader, const char* data, uint32_t shaderType);
		void parseShaders(GLShader* shader, std::string& content, UniformID::USet& uniformArray);
		void parseShaderConfig(GLShader* shader, std::string& content);
		void parseShader(GLShader* shader, std::string& content, const char* regex, uint32_t shaderType, UniformID::USet& uniformArray);
		void splitPasses(ShaderPackage* pack, std::string& content);
		ShaderPackage* parsePackageHead(std::string& content);

	private:
		std::vector<uint32_t> mShaderIDs;
	};


	class TINY_API GLShaderCreator : public ShaderCreator
	{
	public:
		ShaderPackage* create(const std::string& filePath) override;
	};
}

