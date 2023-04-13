#include "Pipeline.h"
#include "../Renderer/RenderPass.h"
#include "../Component/Camera.h"
#include "../Component/MeshRenderer.h"
#include "../Data/Material.h"
#include "../Shader/ShaderPackage.h"
#include "../Shader/Shader.h"
#include "../Manager/PipelineManager.h"
#include "../Renderer/RenderObject.h"

namespace tezcat::Tiny::Core
{
	PipelineQueue* Pipeline::sBackground = new PipelineQueue(PipelineQueue::Background, 1000);
	PipelineQueue* Pipeline::sGeometry = new PipelineQueue(PipelineQueue::Geometry, 2000);
	PipelineQueue* Pipeline::sAlphaTest = new PipelineQueue(PipelineQueue::AlphaTest, 3000);
	PipelineQueue* Pipeline::sOpaqueLast = new PipelineQueue(PipelineQueue::OpaqueLast, 4000);
	PipelineQueue* Pipeline::sTransparent = new PipelineQueue(PipelineQueue::Transparent, 5000);
	PipelineQueue* Pipeline::sOverlay = new PipelineQueue(PipelineQueue::Overlay, 6000);

	Pipeline::Pipeline()
	{

	}

	Pipeline::~Pipeline()
	{

	}

	void Pipeline::addPass(RenderPass* pass)
	{
		Pipeline::addPassStatic(pass);
	}

	void Pipeline::addPassStatic(RenderPass* pass)
	{
		auto shader = pass->getShader();
		switch (shader->getRenderQueue())
		{
		case PipelineQueue::Background:
			sBackground->addPass(pass);
			break;
		case PipelineQueue::Geometry:
			sGeometry->addPass(pass);
			break;
		case PipelineQueue::AlphaTest:
			sAlphaTest->addPass(pass);
			break;
		case PipelineQueue::OpaqueLast:
			sOpaqueLast->addPass(pass);
			break;
		case PipelineQueue::Transparent:
			sTransparent->addPass(pass);
			break;
		case PipelineQueue::Overlay:
			sOverlay->addPass(pass);
			break;
		default:
			break;
		}
	}
}

