#include "MeshRenderer.h"
#include "Transform.h"
#include "Utility/Tools.h"

#include "../Data/Material.h"
#include "../Shader/Shader.h"
#include "../Shader/ShaderPackage.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/VertexGroup.h"
#include "../Pipeline/Pipeline.h"
#include "../Pipeline/PassShader.h"
#include "../Manager/PipelineManager.h"
#include "../Manager/VertexGroupManager.h"
#include "../Renderer/BaseGraphics.h"
#include "../Scene/GameObject.h"

namespace tezcat::Tiny::Core
{
	MeshRenderer::MeshRenderer()
		: MeshRenderer((VertexGroup*)nullptr)
	{

	}

	MeshRenderer::MeshRenderer(MeshData* meshData)
		: MeshRenderer(Graphics->createVertexGroup(meshData))
	{

	}

	MeshRenderer::MeshRenderer(VertexGroup* vg)
		: m_VertexGroup(vg)
		, m_DrawMode(DrawMode::Triangles)
		, m_MainMaterial(nullptr)
		, m_HasIndex(false)
	{
		if (m_VertexGroup)
		{
			m_HasIndex = m_VertexGroup->getIndexCount() > 0;
		}
	}

	MeshRenderer::~MeshRenderer()
	{
		m_VertexGroup = nullptr;
	}

	void MeshRenderer::submit(Shader* shader)
	{
		auto transform = this->getTransform();
		shader->setModelMatrix(transform->getModelMatrix());
		shader->setNormalMatrix(transform->getModelMatrix());
		shader->setTextures(m_MainMaterial->allTexture());
	}

	void MeshRenderer::setMesh(const std::string& meshName)
	{
		m_VertexGroup = VertexGroupMgr::getInstance()->getVertexGroup(meshName);
		m_HasIndex = m_VertexGroup->getIndexCount() > 0;
	}

	void MeshRenderer::sendDataToGPU()
	{
		Graphics->sendMeshData(this->getGameObject()->getLayerMaskIndex(), this);
	}

	void MeshRenderer::onEnable()
	{
		Graphics->addRenderObject(this);
	}

	void MeshRenderer::onStart()
	{

	}

	int MeshRenderer::getVertexCount() const
	{
		return  m_VertexGroup->getVertexCount();
	}

	int MeshRenderer::getIndexCount() const
	{
		return m_VertexGroup->getIndexCount();
	}
}