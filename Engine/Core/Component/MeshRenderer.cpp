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

#include "../Renderer/Vertex.h"
#include "../Renderer/RenderPass.h"
#include "../Renderer/VertexBuffer.h"
#include "../Renderer/ShadowRenderer.h"

#include "../Manager/PipelineManager.h"
#include "../Manager/VertexManager.h"



namespace tezcat::Tiny::Core
{
	MeshRenderer::MeshRenderer()
		: MeshRenderer((Vertex*)nullptr)
	{

	}

	MeshRenderer::MeshRenderer(MeshData* meshData)
		: MeshRenderer(VertexMgr::getInstance()->createVertex(meshData))
	{

	}

	MeshRenderer::MeshRenderer(Vertex* vertex)
		: mVertex(vertex)
		, mDrawMode(ContextMap::DrawModeArray[(uint32_t)DrawMode::Triangles])
		, mMainMaterial(nullptr)
		, mIsCastShadow(true)
	{

	}

	MeshRenderer::~MeshRenderer()
	{
		mVertex = nullptr;
	}

	int MeshRenderer::getVertexCount() const
	{
		return mVertex->getVertexCount();
	}

	int MeshRenderer::getIndexCount() const
	{
		return mVertex->getIndexCount();
	}

	void MeshRenderer::setMesh(const std::string& meshName)
	{
		mVertex = VertexMgr::getInstance()->getVertex(meshName);
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
		mVertex->bind();
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
		mVertex->unbind();
	}
}
