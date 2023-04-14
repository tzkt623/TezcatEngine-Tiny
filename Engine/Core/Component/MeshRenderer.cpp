#include "MeshRenderer.h"
#include "Transform.h"
#include "Utility/Utility.h"

#include "../Data/Material.h"
#include "../Component/GameObject.h"
#include "../Pipeline/Pipeline.h"
#include "../Layer/RenderLayer.h"

#include "../Shader/Shader.h"
#include "../Shader/ShaderPackage.h"
#include "../Shader/Uniform.h"

#include "../Renderer/VertexGroup.h"
#include "../Renderer/RenderPass.h"
#include "../Renderer/VertexBuffer.h"
#include "../Renderer/ShadowRenderer.h"

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
		, mDrawMode(ContextMap::DrawModeArray[(uint32_t)DrawMode::Triangles])
		, mMainMaterial(nullptr)
		, mHasIndex(false)
		, mIsCastShadow(true)
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

	void MeshRenderer::onEnable()
	{

	}

	void MeshRenderer::onDisable()
	{

	}

	void MeshRenderer::onStart()
	{
		RenderLayer::addRenderObejct(this->getGameObject()->getLayerIndex(), this);
	}

	void MeshRenderer::sendToRenderPass(const RenderPassType& passType)
	{
		switch (passType)
		{
		case RenderPassType::Default:
		{
			auto& shaders = mMainMaterial->getShaderPackage()->getShaders();
			for (auto s : shaders)
			{
				RenderPass::get(s)->addRenderMesh(this);
			}
			break;
		}
		case RenderPassType::Shadow:
			if (mIsCastShadow)
			{
				ShadowRenderer::addMesh(this);
			}
			break;
		default:
			break;
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

	void MeshRenderer::submitModelMatrix(Shader* shader)
	{
		auto& model_mat4 = this->getTransform()->getModelMatrix();
		shader->setMat4(ShaderParam::MatrixM, glm::value_ptr(model_mat4));
		glm::mat3 itm(model_mat4);
		itm = glm::inverseTranspose(itm);
		shader->setMat3(ShaderParam::MatrixN, glm::value_ptr(itm));
	}

	void MeshRenderer::endRender()
	{
		mVertexGroup->unbind();
	}
}
