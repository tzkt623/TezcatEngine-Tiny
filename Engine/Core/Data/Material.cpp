#include "Material.h"
#include "../Tool/Tool.h"
#include "../Head/RenderConfig.h"

#include "../Manager/ShaderManager.h"
#include "../Shader/ShaderPackage.h"
#include "../Shader/Shader.h"

namespace tezcat::Tiny
{
	TINY_RTTI_CPP(Material);
	Material::Material(const std::string& name)
		: mName(name)
	{
		mShaderPackage = ShaderMgr::getInstance()->findPackage(name);
	}

	Material::~Material()
	{
		for (auto p : mUniforms)
		{
			delete p;
		}
		mUniforms.clear();
		mShaderPackage = nullptr;
	}

	int Material::getUID() const
	{
		return mShaderPackage->getUID();
	}

	void Material::submit(Shader* shader)
	{
		shader->resetLocalState();
		for (auto uniform : mUniforms)
		{
			uniform->submit(shader);
		}
	}
}
