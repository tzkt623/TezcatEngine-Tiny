#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "Utility/Tools.h"

namespace tezcat::Tiny::Core
{
	class Shader;
	class TINY_API ShaderPackage
	{
	public:
		ShaderPackage();
		~ShaderPackage();

	public:
		void addShader(Shader* shader);
		void setConfig(std::unordered_map<std::string, Any>& data);
		void apply();

	public:
		const std::vector<Shader*>& getShaders() { return m_Shaders; }
		std::string& getName() { return m_Name; }
		int getUID() const { return m_UID; }

	private:
		int m_UID;
		std::string m_Name;
		std::vector<Shader*> m_Shaders;
	};
}