#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Tool/DelegateCreator.h"
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
		void apply();

	public:
		const std::vector<Shader*>& getShaders() { return mShaders; }
		std::string& getName() { return mName; }
		int getUID() const { return mUID; }

	private:
		int mUID;
		std::string mName;
		std::vector<Shader*> mShaders;
	};


	class TINY_API ShaderBuilderCreatorImp
	{
	public:
		virtual ShaderPackage* create(const char* filePath) = 0;
	};

	using ShaderBuilderCreator = DelegateCreator<ShaderBuilderCreatorImp, ShaderPackage>;
}
