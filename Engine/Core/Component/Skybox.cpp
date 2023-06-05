#include "Skybox.h"
#include "GameObject.h"

#include "../Data/Material.h"
#include "../Shader/Shader.h"
#include "../Shader/ShaderPackage.h"
#include "../Renderer/RenderPass.h"
#include "../Renderer/Vertex.h"
#include "../Renderer/BaseGraphics.h"
#include "../Renderer/RenderLayer.h"
#include "../Renderer/RenderCommand.h"

#include "../Manager/VertexBufferManager.h"
#include "../Event/EngineEvent.h"

namespace tezcat::Tiny
{
	TINY_RTTI_CPP(Skybox);

	Skybox::Skybox()
	{
		mRenderAgent->setComponent(this);
	}

	Skybox::~Skybox()
	{

	}

	void Skybox::onStart()
	{
		EngineEvent::get()->dispatch(EventData{EngineEventID::EE_ActiveSkybox, this});
		RenderLayer::addRenderAgent(this->getGameObject()->getLayerIndex(), mRenderAgent);
	}

	void Skybox::onDisable()
	{

	}

	void Skybox::onEnable()
	{
		this->setMesh("Skybox");
	}

	void Skybox::sendToQueue(BaseGraphics* graphics, const RenderPhase& phase, RenderQueue* queue)
	{
		switch (phase)
		{
		case RenderPhase::Forward:
		case RenderPhase::Deferred:
		{
			auto cmd = graphics->createDrawSkyboxCMD(mVertex, this->getTransform(), mMaterial);
			queue->addRenderCommand(cmd);
			break;
		}
		default:
			break;
		}
	}
}
