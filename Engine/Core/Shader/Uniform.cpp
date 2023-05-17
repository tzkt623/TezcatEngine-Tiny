#include "Uniform.h"
#include "Shader.h"

#include "../Head/RenderConfig.h"
#include "../Renderer/Texture.h"
#include "../Component/Transform.h"
#include "../Component/Camera.h"
#include "../Component/GameObject.h"
#include "../Manager/TextureManager.h"

namespace tezcat::Tiny
{
	void UniformI1::submit(Shader* shader)
	{
		shader->setInt1(this->ID, &value);
	}

	void UniformF1::submit(Shader* shader)
	{
		shader->setFloat1(this->ID, &value);
	}

	void UniformI2::submit(Shader* shader)
	{
		shader->setInt2(this->ID, glm::value_ptr(this->value));
	}

	void UniformF2::submit(Shader* shader)
	{
		shader->setFloat2(this->ID, glm::value_ptr(value));
	}

	void UniformI3::submit(Shader* shader)
	{
		shader->setInt3(this->ID, glm::value_ptr(this->value));
	}

	void UniformF3::submit(Shader* shader)
	{
		shader->setFloat3(this->ID, glm::value_ptr(value));
	}

	void UniformI4::submit(Shader* shader)
	{
		shader->setInt4(this->ID, glm::value_ptr(this->value));
	}

	void UniformF4::submit(Shader* shader)
	{
		shader->setFloat4(this->ID, glm::value_ptr(value));
	}

	void UniformMat3::submit(Shader* shader)
	{
		shader->setMat3(this->ID, glm::value_ptr(value));
	}

	void UniformMat4::submit(Shader* shader)
	{
		shader->setMat4(this->ID, glm::value_ptr(value));
	}

	UniformTex2D::UniformTex2D(const UniformID& id, const std::string& name)
		: UniformT(id, (Texture2D*)TextureMgr::getInstance()->findTexture(name))
	{

	}

	void UniformTex2D::submit(Shader* shader)
	{
		shader->setTexture2D(this->ID, this->value);
	}

	UniformTexCube::UniformTexCube(const UniformID& id, const std::string& name)
		: UniformT(id, (TextureCube*)TextureMgr::getInstance()->findTexture(name))
	{

	}

	void UniformTexCube::submit(Shader* shader)
	{
		shader->setTextureCube(this->ID, this->value);
	}

}
