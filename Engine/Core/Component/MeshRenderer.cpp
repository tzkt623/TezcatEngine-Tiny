#include "MeshRenderer.h"
#include "Transform.h"
#include "../Tool/Tool.h"


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
#include "../Renderer/RenderAgent.h"

#include "../Manager/PipelineManager.h"
#include "../Manager/VertexManager.h"



namespace tezcat::Tiny
{
	TINY_RTTI_CPP(MeshRenderer)

	MeshRenderer::MeshRenderer()
		: mDrawMode(ContextMap::DrawModeArray[(uint32_t)DrawMode::Triangles])
		, mMainMaterial(nullptr)
		, mIsCastShadow(true)
		, mRenderAgent(nullptr)
		, mVertex(nullptr)
	{
		
	}

	MeshRenderer::~MeshRenderer()
	{
		mRenderAgent->subRef();
		mVertex->subRef();
		mMainMaterial->subRef();
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
		mVertex->addRef();
	}

	void MeshRenderer::setMesh(MeshData* meshData)
	{
		mVertex = VertexMgr::getInstance()->getVertex(meshData);
		mVertex->addRef();
	}

	void MeshRenderer::onEnable()
	{

	}

	void MeshRenderer::onDisable()
	{
	}

	void MeshRenderer::onStart()
	{
		if (mRenderAgent == nullptr)
		{
			mRenderAgent = RenderAgent::create(this, this);
			mRenderAgent->addRef();
		}
		RenderLayer::addRenderAgent(this->getGameObject()->getLayerIndex(), mRenderAgent);
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
		shader->setMat4(ShaderParam::MatrixM, model_mat4);
		glm::mat3 normal_matrix(model_mat4);
		shader->setMat3(ShaderParam::MatrixN, glm::transpose(glm::inverse(normal_matrix)));
	}

	void MeshRenderer::endRender()
	{
		mVertex->unbind();
	}

	void MeshRenderer::setMaterial(Material* val)
	{
		if (mMainMaterial)
		{
			mMainMaterial->subRef();
		}

		mMainMaterial = val;
		mMainMaterial->addRef();
	}
}
