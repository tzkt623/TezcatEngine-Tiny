#include "ShaderPackage.h"
#include "Shader.h"

#include "Utility/Utility.h"
#include "../Manager/ShaderManager.h"

namespace tezcat::Tiny::Core
{
	ShaderPackage::ShaderPackage()
		: mUID(-1)
	{

	}


	ShaderPackage::~ShaderPackage()
	{

	}

	void ShaderPackage::setConfig(std::unordered_map<std::string, Any>& data)
	{
		mName = data["Name"].cast<std::string>();
	}

	void ShaderPackage::addShader(Shader* shader)
	{
		mShaders.push_back(shader);
	}

	void ShaderPackage::apply()
	{
		mUID = ShaderMgr::getInstance()->giveUID();
		this->sort();
	}

	void ShaderPackage::sort()
	{
		if (mShaders.size() > 1)
		{
			std::sort(mShaders.begin(), mShaders.end(),
				[](Shader* a, Shader* b)
				{
					return a->getOrderID() < b->getOrderID();
				});
		}
	}

	void ShaderPackage::rebuild()
	{
		ShaderMgr::getInstance()->rebuild(this);
	}

	Shader* ShaderPackage::findShader(const std::string& name)
	{
		auto it = std::find_if(mShaders.begin(), mShaders.end(), [&](Shader* a)
		{
			return a->getName() == name;
		});

		if (it != mShaders.end())
		{
			return *it;
		}

		return nullptr;
	}
}
