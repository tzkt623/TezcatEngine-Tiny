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
		: Material(ShaderMgr::getInstance()->find(name))
	{

	}

	Material::Material(Shader* shader)
		: mShader(shader)
	{
		TinyAssert(mShader != nullptr);

		mShader->addRef();

		auto& umap = mShader->getUniformMap();
		mUniforms.resize(umap.size(), nullptr);

		for (auto& pair : umap)
		{
			Uniform* uniform = nullptr;

			auto info = pair.second;
			switch (info->type)
			{
			case UniformType::Bool:
			{
				uniform = new UniformI1(&info->shaderID);
				break;
			}
			case UniformType::Int:
			{
				uniform = new UniformI1(&info->shaderID);
				break;
			}
			case UniformType::Int2:
			{
				uniform = new UniformI2(&info->shaderID);
				break;
			}
			case UniformType::Int3:
			{
				uniform = new UniformI3(&info->shaderID);
				break;
			}
			case UniformType::Int4:
			{
				uniform = new UniformI4(&info->shaderID);
				break;
			}
			case UniformType::Float:
			{
				uniform = new UniformF1(&info->shaderID);
				break;
			}
			case UniformType::Float2:
			{
				uniform = new UniformF2(&info->shaderID);
				break;
			}
			case UniformType::Float3:
			{
				uniform = new UniformF3(&info->shaderID);
				break;
			}
			case UniformType::Float4:
			{
				uniform = new UniformF4(&info->shaderID);
				break;
			}
			case UniformType::Mat3:
			{
				uniform = new UniformMat3(&info->shaderID);
				break;
			}
			case UniformType::Mat4:
			{
				uniform = new UniformMat4(&info->shaderID);
				break;
			}
			case UniformType::Tex2D:
			{
				uniform = new UniformTex2D(&info->shaderID);
				break;
			}
			case UniformType::Tex3D:
			{
				//uniform = new UniformTex3D(&info->shaderID);
				break;
			}
			case UniformType::TexCube:
			{
				uniform = new UniformTexCube(&info->shaderID);
				break;
			}
			default:
				break;
			}

			mUniforms[info->index] = uniform;
		}
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
