#include "MeshRenderer.h"
#include "Transform.h"
#include "Utility/Utility.h"

#include "../Data/Material.h"
#include "../Component/GameObject.h"
#include "../Pipeline/Pipeline.h"

#include "../Shader/Shader.h"
#include "../Shader/ShaderPackage.h"
#include "../Shader/Uniform.h"

#include "../Renderer/VertexGroup.h"
#include "../Renderer/RenderPass.h"
#include "../Renderer/VertexBuffer.h"

#include "../Manager/PipelineManager.h"
#include "../Manager/VertexGroupManager.h"



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
		, mIsCastShadow(false)
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

	int MeshRenderer::getVertexCount() const
	{
		return mVertexGroup->getVertexCount();
	}

	int MeshRenderer::getIndexCount() const
	{
		return mVertexGroup->getIndexCount();
	}

	void MeshRenderer::setMesh(const std::string& meshName)
	{
		mVertexGroup = VertexGroupMgr::getInstance()->getVertexGroup(meshName);
		mHasIndex = mVertexGroup->getIndexCount() > 0;
	}

	void MeshRenderer::addMaterialConfig()
	{
		//		m_MainMaterial->addUniform<UniformMode>();
		mMainMaterial->addUniform<UniformMatrixM>(ShaderParam::MatrixM);
		mMainMaterial->addUniform<UniformMatrixN>(ShaderParam::MatrixN);
	}

	void MeshRenderer::onEnable()
	{
	}

	void MeshRenderer::onDisable()
	{

	}

	void MeshRenderer::onStart()
	{
		if (mIsCastShadow)
		{
			PipelineMgr::getInstance()->addCastShadowObject(this);
		}

		PipelineMgr::getInstance()->addRenderObject(this->getGameObject()->getLayerIndex(), this);
	}

	void MeshRenderer::sendToRenderPass()
	{
		auto& shaders = mMainMaterial->getShaderPackage()->getShaders();
		for (auto s : shaders)
		{
			PipelineMgr::getInstance()
				->getPass(s->getUID())
				->addRenderMesh(this);
		}
	}

	void MeshRenderer::beginRender()
	{
		mVertexGroup->bind();
	}

	void MeshRenderer::submit(Shader* shader)
	{
		mMainMaterial->submit(this->getTransform(), shader);
	}

	void MeshRenderer::endRender()
	{
		mVertexGroup->unbind();
	}
}
