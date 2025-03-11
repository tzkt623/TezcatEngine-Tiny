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

#include "Core/Shader/Uniform.h"
#include "Core/Shader/Shader.h"

#include "Core/Renderer/Texture.h"
#include "Core/Renderer/BaseGraphics.h"
#include "Core/Renderer/RenderConfig.h"

#include "Core/Component/Transform.h"
#include "Core/Component/Camera.h"
#include "Core/Component/GameObject.h"

#include "Core/Manager/TextureManager.h"

namespace tezcat::Tiny
{
	void UniformI1::submit(Shader* shader)
	{
		Graphics::getInstance()->setInt1(shader, *mValueID, &this->value);
	}

	void UniformF1::submit(Shader* shader)
	{
		Graphics::getInstance()->setFloat1(shader, *mValueID, &this->value);
	}

	void UniformI2::submit(Shader* shader)
	{
		Graphics::getInstance()->setInt2(shader, *mValueID, glm::value_ptr(this->value));
	}

	void UniformF2::submit(Shader* shader)
	{
		Graphics::getInstance()->setFloat2(shader, *mValueID, glm::value_ptr(this->value));
	}

	void UniformI3::submit(Shader* shader)
	{
		Graphics::getInstance()->setInt3(shader, *mValueID, glm::value_ptr(this->value));
	}

	void UniformF3::submit(Shader* shader)
	{
		Graphics::getInstance()->setFloat3(shader, *mValueID, glm::value_ptr(this->value));
	}

	void UniformI4::submit(Shader* shader)
	{
		Graphics::getInstance()->setInt4(shader, *mValueID, glm::value_ptr(this->value));
	}

	void UniformF4::submit(Shader* shader)
	{
		Graphics::getInstance()->setFloat4(shader, *mValueID, glm::value_ptr(this->value));
	}

	void UniformMat3::submit(Shader* shader)

	{
		Graphics::getInstance()->setMat3(shader, *mValueID, glm::value_ptr(this->value));
	}

	void UniformMat4::submit(Shader* shader)
	{
		Graphics::getInstance()->setMat4(shader, *mValueID, glm::value_ptr(this->value));
	}

	UniformTex2D::~UniformTex2D()
	{
		this->value->deleteObject();
	}

	void UniformTex2D::submit(Shader* shader)
	{
		Graphics::getInstance()->setTexture2D(shader, *mValueID, this->value);
	}

	UniformTexCube::~UniformTexCube()
	{
		this->value->deleteObject();
	}

	void UniformTexCube::submit(Shader* shader)
	{
		Graphics::getInstance()->setTextureCube(shader, *mValueID, this->value);
	}
}
