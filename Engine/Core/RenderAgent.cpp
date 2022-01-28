#include "RenderAgent.h"
#include "Engine.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Material.h"
#include "VAOManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Shader.h"
#include "Texture.h"

namespace tezcat::Tiny::Core
{
	RenderAgent::RenderAgent() :
		m_Mesh(nullptr),
		m_Material(nullptr),
		m_MasterObject(nullptr),
		m_Enable(true)
	{

	}

	RenderAgent::~RenderAgent()
	{
		m_Material = nullptr;
		m_Mesh = nullptr;
		m_MasterObject = nullptr;
	}

	void RenderAgent::apply()
	{
		Core::Engine::getInstance()->getRenderer()->dispatch(this);
	}

	void RenderAgent::render(Shader* shader)
	{
		auto transform = this->getMasterObject()->getTransform();
		shader->setModelMatrix(transform->getModelMatrix());
		shader->setTextures(m_Material->allTexture());
		m_Mesh->render(shader);
	}

	void RenderAgent::setMeshData(MeshData* meshData)
	{
		m_Mesh = new Mesh(meshData);
	}

	void RenderAgent::setMesh(const std::string& name)
	{
		auto vao = VAOManager::getInstance()->getVAO(name);
		if (vao)
		{
			m_Mesh = new Mesh(vao);
		}
	}
}