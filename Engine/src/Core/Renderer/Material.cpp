/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Core/Renderer/Material.h"
#include "Core/Tool/Tool.h"
#include "Core/Renderer/RenderConfig.h"
		 
#include "Core/Manager/ShaderManager.h"
#include "Core/Shader/Shader.h"

namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(Material, TinyObject)
	Material::Material(std::string name)
		: mShader(nullptr)
		, mName(std::move(name))
	{
		this->setShader(ShaderManager::find(mName));
	}

	Material::Material(Shader* shader)
		: mShader(nullptr)
	{
		this->setShader(shader);
		mName = shader->getName();
	}

	Material::~Material()
	{
		for (auto p : mUniforms)
		{
			delete p;
		}

		mUniforms.clear();
		mShader->deleteObject();
	}

	int32_t Material::getUID() const
	{
		return mShader->getUID();
	}

	void Material::submit(Shader* shader)
	{
		shader->resetLocalState();
		for (auto uniform : mUniforms)
		{
			uniform->submit(shader);
		}
	}

	void Material::setShader(Shader* shader)
	{
		if (mShader)
		{
			mShader->deleteObject();
			for (auto uniform : mUniforms)
			{
				if (uniform)
				{
					delete uniform;
				}
			}
			mUniforms.clear();
		}

		mShader = shader;
		mShader->saveObject();

		auto& umap = mShader->getUserUniformMap();
		mUniforms.resize(umap.size(), nullptr);

		for (auto& pair : umap)
		{
			Uniform* uniform = nullptr;

			auto info = pair.second;
			switch (info->type)
			{
			case UniformType::Bool:
			{
				uniform = new UniformI1(&info->valueID);
				break;
			}
			case UniformType::Int:
			{
				uniform = new UniformI1(&info->valueID);
				break;
			}
			case UniformType::Int2:
			{
				uniform = new UniformI2(&info->valueID);
				break;
			}
			case UniformType::Int3:
			{
				uniform = new UniformI3(&info->valueID);
				break;
			}
			case UniformType::Int4:
			{
				uniform = new UniformI4(&info->valueID);
				break;
			}
			case UniformType::Float:
			{
				uniform = new UniformF1(&info->valueID);
				break;
			}
			case UniformType::Float2:
			{
				uniform = new UniformF2(&info->valueID);
				break;
			}
			case UniformType::Float3:
			{
				uniform = new UniformF3(&info->valueID);
				break;
			}
			case UniformType::Float4:
			{
				uniform = new UniformF4(&info->valueID);
				break;
			}
			case UniformType::Mat3:
			{
				uniform = new UniformMat3(&info->valueID);
				break;
			}
			case UniformType::Mat4:
			{
				uniform = new UniformMat4(&info->valueID);
				break;
			}
			case UniformType::Tex2D:
			{
				uniform = new UniformTex2D(&info->valueID);
				break;
			}
			case UniformType::Tex3D:
			{
				//uniform = new UniformTex3D(&info->valueID);
				break;
			}
			case UniformType::TexCube:
			{
				uniform = new UniformTexCube(&info->valueID);
				break;
			}
			default:
				break;
			}

			mUniforms[info->index] = uniform;
		}
	}
}
