#pragma once

#include "../Head/CppHead.h"
#include "Utility/Utility.h"

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
		void sort();
		void apply();
		void rebuild();

	public:
		const std::vector<Shader*>& getShaders() { return mShaders; }
		std::string& getName() { return mName; }
		int getUID() const { return mUID; }
		Shader* findShader(const std::string& name);

	private:
		int mUID;
		std::string mName;
		std::vector<Shader*> mShaders;
	};
}
