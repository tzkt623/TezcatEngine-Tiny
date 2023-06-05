#include "Material.h"
#include "../Tool/Tool.h"
#include "../Head/RenderConfig.h"

#include "../Manager/ShaderManager.h"
#include "../Shader/Shader.h"
#include "../Renderer/BaseGraphics.h"

namespace tezcat::Tiny
{
	TINY_RTTI_CPP(Material);
	Material::Material(const std::string& name)
		: mName(name)
	{
		mShader = ShaderMgr::getInstance()->find(name);
		mShader->addRef();
	}

	Material::~Material()
	{
		for (auto p : mUniforms)
		{
			delete p;
		}
		mUniforms.clear();
		mShader->subRef();
	}

	int Material::getUID() const
	{
		return mShader->getUID();
	}

	void Material::submit(BaseGraphics* graphics, Shader* shader)
	{
		shader->resetLocalState();
		for (auto uniform : mUniforms)
		{
			uniform->submit(graphics, shader);
		}
	}
}
