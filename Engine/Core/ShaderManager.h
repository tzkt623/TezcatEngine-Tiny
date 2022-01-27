#pragma once
#include <unordered_map>
#include <string>
#include "Tools.h"

namespace tezcat::Tiny::Core
{
	class Shader;
	class ShaderManager
	{
		CREATE_DELEGATE_INSTANCE(ShaderManager)
	public:
		ShaderManager();

		~ShaderManager();

	public:
		void addShader(Shader* shader);
		Shader* find(const std::string& name);


	private:
		std::unordered_map<std::string, Shader*> m_ShaderMap;
	};

}

