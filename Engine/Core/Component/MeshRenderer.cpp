#include "MeshRenderer.h"
#include "Transform.h"
#include "../Tool/Tool.h"


#include "../Data/Material.h"
#include "../Component/GameObject.h"

#include "../Shader/Shader.h"
#include "../Shader/ShaderPackage.h"
#include "../Shader/Uniform.h"

#include "../Renderer/Vertex.h"
#include "../Renderer/RenderPass.h"
#include "../Renderer/VertexBuffer.h"
#include "../Renderer/ShadowRenderer.h"
#include "../Renderer/RenderAgent.h"
#include "../Renderer/RenderLayer.h"
#include "../Renderer/RenderCommand.h"

#include "../Manager/ShadowCasterManager.h"
#include "../Manager/BufferManager.h"


namespace tezcat::Tiny
{
	TINY_RTTI_CPP(MeshRenderer);

	MeshRenderer::MeshRenderer()
		: mIsCastShadow(true)
	{
		mRenderAgent->setComponent(this);
	}

	MeshRenderer::~MeshRenderer()
	{

	}

	void MeshRenderer::onEnable()
	{

	}

	void MeshRenderer::onDisable()
	{

	}

	void MeshRenderer::onStart()
	{
		RenderLayer::addRenderAgent(this->getGameObject()->getLayerIndex(), mRenderAgent);
	}

	void MeshRenderer::sendToQueue(const RenderPhase& phase, RenderQueue* queue)
	{
		switch (phase)
		{
		case RenderPhase::Shadow:
		{
			if (!mIsCastShadow)
			{
				break;
			}

			queue->addRenderCommand(new RenderCMD_Shadow(mVertex, this->getTransform()));
			break;
		}
		case RenderPhase::Forward:
		{
			queue->addRenderCommand(new RenderCMD_Mesh(mVertex, this->getTransform(), mMaterial));
			break;
		}
		default:
			break;
		}
	}

}
