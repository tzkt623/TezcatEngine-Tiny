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
	Pipeline::Pipeline()
		: mGraphics(nullptr)
	{

	}

	Pipeline::~Pipeline()
	{
		mGraphics = nullptr;
	}
}

