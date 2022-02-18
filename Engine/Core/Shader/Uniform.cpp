#include "Uniform.h"
#include "../Head/Context.h"
#include "../Renderer/Texture.h"
#include "../Component/Transform.h"
#include "Shader.h"
#include "../Component/Camera.h"
#include "../Scene/GameObject.h"
#include "../Manager/TextureManager.h"

namespace tezcat::Tiny::Core
{
	UniformTex2D::UniformTex2D(const UniformID& id, const char* filePath)
		: UniformT(id, (Texture2D*)TextureMgr::getInstance()->tryGetTexture(filePath, TextureType::Texture2D))
	{

	}

	void UniformTex2D::submit(Transform* transform, Shader* shader)
	{
		shader->setTexture2D(this->ID, this->value);
	}

	void UniformI1::submit(Transform* transform, Shader* shader)
	{
		shader->setInt1(this->ID, &value);
	}

	void UniformF1::submit(Transform* transform, Shader* shader)
	{
		shader->setFloat1(this->ID, &value);
	}

	void UniformI2::submit(Transform* transform, Shader* shader)
	{
		shader->setInt2(this->ID, glm::value_ptr(this->value));
	}

	void UniformF2::submit(Transform* transform, Shader* shader)
	{
		shader->setFloat2(this->ID, glm::value_ptr(value));
	}

	void UniformI3::submit(Transform* transform, Shader* shader)
	{
		shader->setInt3(this->ID, glm::value_ptr(this->value));
	}

	void UniformF3::submit(Transform* transform, Shader* shader)
	{
		shader->setFloat3(this->ID, glm::value_ptr(value));
	}

	void UniformI4::submit(Transform* transform, Shader* shader)
	{
		shader->setInt4(this->ID, glm::value_ptr(this->value));
	}

	void UniformF4::submit(Transform* transform, Shader* shader)
	{
		shader->setFloat4(this->ID, glm::value_ptr(value));
	}

	void UniformMat3::submit(Transform* transform, Shader* shader)
	{
		shader->setMat3(this->ID, glm::value_ptr(value));
	}

	void UniformMat4::submit(Transform* transform, Shader* shader)
	{
		shader->setMat4(this->ID, glm::value_ptr(value));
	}


	//---------------------------------------------------------
	//
	//	ÌØ»¯Uniform
	//

	void UniformMatrixP::submit(Transform* transform, Shader* shader)
	{
		auto camera = transform->getGameObject()->getComponent<Camera>();
		shader->setMat4(this->ID, glm::value_ptr(camera->getProjectionMatrix()));
	}

	void UniformMatrixV::submit(Transform* transform, Shader* shader)
	{
		auto camera = transform->getGameObject()->getComponent<Camera>();
		shader->setMat4(this->ID, glm::value_ptr(camera->getViewMatrix()));
	}

	void UniformMatrixM::submit(Transform* transform, Shader* shader)
	{
		shader->setMat4(this->ID, glm::value_ptr(transform->getModelMatrix()));
	}

	void UniformMatrixN::submit(Transform* transform, Shader* shader)
	{
		auto mat = glm::mat3(glm::transpose(glm::inverse(transform->getModelMatrix())));
		shader->setMat3(this->ID, glm::value_ptr(mat));
	}
}