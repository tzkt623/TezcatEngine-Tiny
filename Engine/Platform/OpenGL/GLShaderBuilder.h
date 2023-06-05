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

		void parseHeader(std::string& content);
		void parseShaders(std::string& content, std::string& rootPath);
		void parseShader(std::string& content, std::string& rootPath, const char* regex, UniformID::USet& uniformArray, std::string& shaderContent);
		void splitPasses(std::string& content);
		void updateShaderConfig(Shader* shader);
		std::string& getVertexShader() { return mVertexShader; }
		std::string& getFragShader() { return mFragShader; }

	public:
		std::unordered_map<std::string, Any> mConfigUMap;
		std::vector<std::string> mPassData;
		std::unordered_map<std::string, Any> mHeadUMap;
		UniformID::USet mUniformSet;

		std::string mVertexShader;
		std::string mFragShader;

	private:
		std::vector<uint32_t> mShaderIDs;
	};
}

