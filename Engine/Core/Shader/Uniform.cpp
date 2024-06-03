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
		graphics->setInt1(shader, *mShaderID, &value);
	}

	void UniformF1::submit(BaseGraphics* graphics, Shader* shader)
	{
		graphics->setFloat1(shader, *mShaderID, &value);
	}

	void UniformI2::submit(BaseGraphics* graphics, Shader* shader)
	{
		graphics->setInt2(shader, *mShaderID, glm::value_ptr(this->value));
	}

	void UniformF2::submit(BaseGraphics* graphics, Shader* shader)
	{
		graphics->setFloat2(shader, *mShaderID, glm::value_ptr(value));
	}

	void UniformI3::submit(BaseGraphics* graphics, Shader* shader)
	{
		graphics->setInt3(shader, *mShaderID, glm::value_ptr(this->value));
	}

	void UniformF3::submit(BaseGraphics* graphics, Shader* shader)
	{
		graphics->setFloat3(shader, *mShaderID, glm::value_ptr(value));
	}

	void UniformI4::submit(BaseGraphics* graphics, Shader* shader)
	{
		graphics->setInt4(shader, *mShaderID, glm::value_ptr(this->value));
	}

	void UniformF4::submit(BaseGraphics* graphics, Shader* shader)
	{
		graphics->setFloat4(shader, *mShaderID, glm::value_ptr(value));
	}

	void UniformMat3::submit(BaseGraphics* graphics, Shader* shader)

	{
		graphics->setMat3(shader, *mShaderID, glm::value_ptr(value));
	}

	void UniformMat4::submit(BaseGraphics* graphics, Shader* shader)
	{
		graphics->setMat4(shader, *mShaderID, glm::value_ptr(value));
	}

	UniformTex2D::~UniformTex2D()
	{
		this->value->deleteObject();
	}

	void UniformTex2D::submit(BaseGraphics* graphics, Shader* shader)
	{
		graphics->setTexture2D(shader, *mShaderID, this->value);
	}

	void UniformTex2D::beginSetValue()
	{
		if (this->value)
		{
			this->value->deleteObject();
		}
	}

	void UniformTex2D::endSetValue()
	{
		this->value->saveObject();
	}

	UniformTexCube::~UniformTexCube()
	{
		this->value->deleteObject();
	}

	void UniformTexCube::submit(BaseGraphics* graphics, Shader* shader)
	{
		graphics->setTextureCube(shader, *mShaderID, this->value);
	}

	void UniformTexCube::beginSetValue()
	{
		if (this->value)
		{
			this->value->deleteObject();
		}
	}

	void UniformTexCube::endSetValue()
	{
		this->value->saveObject();
	}

}
