#pragma once
#include "Core/Head/CppHead.h"
#include "Core/Head/ConfigHead.h"
#include "Core/Shader/Uniform.h"
#include "Core/Shader/Shader.h"

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
		void parseShader(std::string& content, std::string& rootPath, const char* regex, std::string& shaderContent);
		void splitPasses(std::string& content);
		void updateShaderConfig(Shader* shader);
		std::string& getVertexShader() { return mVertexShader; }
		std::string& getFragShader() { return mFragShader; }

	private:
		static bool splitConfig(const std::string& content, std::string& config, std::string& suffix, const char* regex);
		static bool splitValue(std::string& content, std::unordered_map<std::string, Any>& map);

	public:
		std::unordered_map<std::string, Any> mConfigUMap;
		std::vector<std::string> mPassData;
		std::unordered_map<std::string, Any> mHeadUMap;
		UniformID::USet mUniformSet;
		std::unordered_set<std::string> mUserSet;

		std::unordered_map<std::string, UniformType> mTinyUMap;
		std::unordered_map<std::string, UniformType> mUserUMap;

		std::string mVertexShader;
		std::string mFragShader;

	private:
		std::vector<uint32_t> mShaderIDs;
	};
}

