#include "Uniform.h"
#include "Shader.h"

#include "../Head/RenderConfig.h"
#include "../Renderer/Texture.h"
#include "../Component/Transform.h"
#include "../Component/Camera.h"
#include "../Component/GameObject.h"
#include "../Manager/TextureManager.h"
#include "../Renderer/BaseGraphics.h"

namespace tezcat::Tiny
{
	void UniformI1::submit(BaseGraphics* graphics, Shader* shader)
	{
		graphics->setInt1(shader, this->ID, &value);
	}

	void UniformF1::submit(BaseGraphics* graphics, Shader* shader)
	{
		graphics->setFloat1(shader, this->ID, &value);
	}

	void UniformI2::submit(BaseGraphics* graphics, Shader* shader)
	{
		graphics->setInt2(shader, this->ID, glm::value_ptr(this->value));
	}

	void UniformF2::submit(BaseGraphics* graphics, Shader* shader)
	{
		graphics->setFloat2(shader, this->ID, glm::value_ptr(value));
	}

	void UniformI3::submit(BaseGraphics* graphics, Shader* shader)
	{
		graphics->setInt3(shader, this->ID, glm::value_ptr(this->value));
	}

	void UniformF3::submit(BaseGraphics* graphics, Shader* shader)
	{
		graphics->setFloat3(shader, this->ID, glm::value_ptr(value));
	}

	void UniformI4::submit(BaseGraphics* graphics, Shader* shader)
	{
		graphics->setInt4(shader, this->ID, glm::value_ptr(this->value));
	}

	void UniformF4::submit(BaseGraphics* graphics, Shader* shader)
	{
		graphics->setFloat4(shader, this->ID, glm::value_ptr(value));
	}

	void UniformMat3::submit(BaseGraphics* graphics, Shader* shader)
	{
		graphics->setMat3(shader, this->ID, glm::value_ptr(value));
	}

	void UniformMat4::submit(BaseGraphics* graphics, Shader* shader)
	{
		graphics->setMat4(shader, this->ID, glm::value_ptr(value));
	}

	UniformTex2D::UniformTex2D(const UniformID& id, const std::string& name)
		: UniformT(id, (Texture2D*)TextureMgr::getInstance()->find(name))
	{
		this->value->addRef();
	}

	UniformTex2D::~UniformTex2D()
	{
		this->value->subRef();
	}

	void UniformTex2D::submit(BaseGraphics* graphics, Shader* shader)
	{
		graphics->setTexture2D(shader, this->ID, this->value);
	}

	void UniformTex2D::beginSetValue()
	{
		if (this->value)
		{
			this->value->subRef();
		}
	}

	void UniformTex2D::endSetValue()
	{
		this->value->addRef();
	}

	UniformTexCube::UniformTexCube(const UniformID& id, const std::string& name)
		: UniformT(id, (TextureCube*)TextureMgr::getInstance()->find(name))
	{
		this->value->addRef();
	}

	UniformTexCube::~UniformTexCube()
	{
		this->value->subRef();
	}

	void UniformTexCube::submit(BaseGraphics* graphics, Shader* shader)
	{
		graphics->setTextureCube(shader, this->ID, this->value);
	}

	void UniformTexCube::beginSetValue()
	{
		if (this->value)
		{
			this->value->subRef();
		}
	}

	void UniformTexCube::endSetValue()
	{
		this->value->addRef();
	}

}
