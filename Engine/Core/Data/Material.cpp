#include "Material.h"
#include "Utility/Utility.h"

#include "../Manager/ShaderManager.h"
#include "../Shader/ShaderPackage.h"
#include "../Head/Context.h"
#include "../Renderer/BaseGraphics.h"
#include "../Shader/Shader.h"
#include "../Component/Transform.h"

namespace tezcat::Tiny::Core
{
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

	void Material::submit(Transform* transform, Shader* shader)
	{
		shader->resetState();
		for (auto uniform : mUniforms)
		{
			uniform->submit(transform, shader);
		}
	}
}
