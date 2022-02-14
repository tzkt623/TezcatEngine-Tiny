#pragma once
#include "../Head/CppHead.h"
#include "../Head/GLHead.h"
#include "../Head/ConfigHead.h"

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
	class ShaderPackage;
	class TINY_API ShaderBuilder
	{
		struct Head
		{

		};

		struct Pass
		{

		};

	private:
		ShaderBuilder();
		~ShaderBuilder();

	private:
		Shader* loadFromFile(const char* filePath);
		void loadFromData(Shader* shader, const char* data, GLenum shaderType);

		void parseShaders(Shader* shader, std::string& content);
		void parseShaderConfig(Shader* shader, std::string& content);
		void parseShader(Shader* shader, std::string& content, const char* regex, uint32_t shaderType);
		void splitPackage(std::string& content);
		void splitPasses(ShaderPackage* pack, std::string& content);
		ShaderPackage* parsePackageHead(std::string& content);

	public:
		static void createPackage(const std::string& filePath);
	private:
		std::vector<uint32_t> m_ShaderIDs;
	};
}

