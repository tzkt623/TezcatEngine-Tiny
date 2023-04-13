#include "ShaderManager.h"
#include "../Shader/Shader.h"
#include "../Shader/ShaderPackage.h"

namespace tezcat::Tiny::Core
{
	ShaderManager::ShaderManager()
	{
		SG<ShaderManager>::attach(this);
	}

	ShaderManager::~ShaderManager()
	{

	}

	int ShaderManager::giveUID()
	{
		int uid = static_cast<int>(mShaderPackageAry.size());
		mShaderPackageAry.push_back(nullptr);
		return uid;
	}

	void ShaderManager::addShader(Shader* shader)
	{
		while (mAllShaderList.size() <= shader->getUID())
		{
			mAllShaderList.push_back(nullptr);
		}
		mAllShaderList[shader->getUID()] = shader;
	}

	void ShaderManager::addShaderPackage(ShaderPackage* shaderPackage)
	{
		if (mShaderPackageAry[shaderPackage->getUID()] == nullptr)
		{
			mShaderPackageAry[shaderPackage->getUID()] = shaderPackage;
			mShaderPackageDict.emplace(shaderPackage->getName(), shaderPackage);
		}
		else
		{
			throw std::invalid_argument(StringTool::stringFormat("ShaderManager: This ShaderPackage[%s] has stored[%d]",
				shaderPackage->getName().c_str(), shaderPackage->getUID()));
		}
	}

	ShaderPackage* ShaderManager::findPackage(const std::string& name)
	{
		auto it = mShaderPackageDict.find(name);
		if (it != mShaderPackageDict.end())
		{
			return it->second;
		}

		return nullptr;
	}
}

