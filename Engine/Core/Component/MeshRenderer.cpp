#include "MeshRenderer.h"
#include "Transform.h"
#include "Utility/Utility.h"

#include "../Data/Material.h"
#include "../Shader/Shader.h"
#include "../Shader/ShaderPackage.h"
#include "../Renderer/VertexGroup.h"
#include "../Pipeline/Pipeline.h"
#include "../Renderer/RenderPass.h"
#include "../Manager/PipelineManager.h"
#include "../Manager/VertexGroupManager.h"
#include "../Renderer/BaseGraphics.h"
#include "../Component/GameObject.h"
#include "../Shader/Uniform.h"
#include "../Renderer/VertexBuffer.h"

namespace tezcat::Tiny::Core
{
	MeshRenderer::MeshRenderer()
		: MeshRenderer((VertexGroup*)nullptr)
	{

	}

	MeshRenderer::MeshRenderer(MeshData* meshData)
		: MeshRenderer(VertexGroupCreator::create(meshData))
	{

	}

	MeshRenderer::MeshRenderer(VertexGroup* vg)
		: mVertexGroup(vg)
		, mDrawMode(ContextMap::DrawModeArray[(int)DrawMode::Triangles])
		, mMainMaterial(nullptr)
		, mHasIndex(false)
	{
		if (mVertexGroup)
		{
			mHasIndex = mVertexGroup->getIndexCount() > 0;
		}
	}

	MeshRenderer::~MeshRenderer()
	{
		mVertexGroup = nullptr;
	}

	void MeshRenderer::submit(Shader* shader)
	{
		mVertexGroup->bind();
		mMainMaterial->submit(this->getTransform(), shader);
		Graphics::getInstance()->draw(this);
		mVertexGroup->unbind();
	}

	void MeshRenderer::sendToRenderPass()
	{
		auto& shaders = mMainMaterial->getShaderPackage()->getShaders();
		for (auto s : shaders)
		{
			PipelineManager::getPass(s->getUID())->addRenderObject(this);
		}
	}

	void MeshRenderer::setMesh(const std::string& meshName)
	{
		mVertexGroup = VertexGroupMgr::getInstance()->getVertexGroup(meshName);
		mHasIndex = mVertexGroup->getIndexCount() > 0;
	}

	void MeshRenderer::onEnable()
	{
		PipelineManager::addRenderObject(this->getGameObject()->getLayerMaskIndex(), this);
	}

	void MeshRenderer::onDisable()
	{

	}

	void MeshRenderer::onStart()
	{

	}

	int MeshRenderer::getVertexCount() const
	{
		return mVertexGroup->getVertexCount();
	}

	int MeshRenderer::getIndexCount() const
	{
		return mVertexGroup->getIndexCount();
	}

	void MeshRenderer::addMaterialConfig()
	{
		//		m_MainMaterial->addUniform<UniformMode>();
		mMainMaterial->addUniform<UniformMatrixM>(ShaderParam::MatrixM);
		mMainMaterial->addUniform<UniformMatrixN>(ShaderParam::MatrixN);
	}
}
