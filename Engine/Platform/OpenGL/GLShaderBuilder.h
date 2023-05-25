#pragma once
#include "Core/Head/CppHead.h"
#include "Core/Head/ConfigHead.h"
#include "Core/Shader/Uniform.h"
#include "Core/Manager/ShaderManager.h"

namespace tezcat::Tiny
{
	class Shader;
}

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

		/// <summary>
		/// 分离package得到各个pass和pack config
		/// </summary>
		void splitPackage(ShaderPackage *packge, std::string& content);
		void splitPasses(std::string& content);
		void parseShaders(GLShader* shader, std::string& content, UniformID::USet& uniformArray);
		Shader* parseShaders(ShaderPackage* package, std::string& inContent, std::string& outShaderContent);
		void reparseShader(Shader* shader, std::string& content, UniformID::USet& uniformArray);

	private:
		void loadFromData(GLShader* shader, const char* data, uint32_t shaderType);
		void parseShaderConfig(GLShader* shader, std::string& content);
		void parseShader(GLShader* shader, std::string& content, const char* regex, uint32_t shaderType, UniformID::USet& uniformArray);
		void splitPasses(GLShader* shader, std::string& content);
		void parsePackageHead(ShaderPackage* packge, std::string& content);
		void setShaderConfig(Shader* shader, std::unordered_map<std::string, Any>& map);

	public:
		std::unordered_map<std::string, Any> mConfigUMap;
		std::vector<std::string> mPassData;


	private:
		std::vector<uint32_t> mShaderIDs;
	};


	class TINY_API GLShaderCreator : public ShaderCreator
	{
	public:
		ShaderPackage* create(const std::string& filePath) override;
		void rebuild(ShaderPackage* package, std::string& data) override;

	private:
		std::unordered_map<std::string, std::string> mShaderPath;
	};
}

