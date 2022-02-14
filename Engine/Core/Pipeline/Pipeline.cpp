#include "Pipeline.h"
#include "PassShader.h"
#include "../Component/Camera.h"
#include "../Component/MeshRenderer.h"
#include "../Data/Material.h"
#include "../Shader/ShaderPackage.h"
#include "../Shader/Shader.h"
#include "../Manager/PipelineManager.h"

namespace tezcat::Tiny::Core
{
	std::unordered_map<std::string, Pipeline::Queue> Pipeline::s_QueueMap =
	{
		{"Background",	Queue::Background},
		{"Opaque",		Queue::Opaque},
		{"Alpha",		Queue::Alpha},
		{"OpaqueLast",	Queue::OpaqueLast},
		{"Transparent", Queue::Transparent},
		{"Overlay",		Queue::Overlay}
	};

	void Pipeline::render(Camera* camera)
	{
		for (auto pass : m_ShaderList)
		{
			pass->render(camera);
		}
	}

	void Pipeline::addPass(PassShader* pass)
	{
		pass->attach();
		m_ShaderList.push_back(pass);
	}


	PipelineGroup::PipelineGroup()
	{
	}

	PipelineGroup::~PipelineGroup()
	{

	}

	void PipelineGroup::init()
	{
		m_PipelineList.push_back(new Pipeline(Pipeline::Background));
		m_PipelineList.push_back(new Pipeline(Pipeline::Opaque));
		m_PipelineList.push_back(new Pipeline(Pipeline::Alpha));
		m_PipelineList.push_back(new Pipeline(Pipeline::OpaqueLast));
		m_PipelineList.push_back(new Pipeline(Pipeline::Transparent));
		m_PipelineList.push_back(new Pipeline(Pipeline::Overlay));
	}
	void PipelineGroup::render(Camera* camera)
	{
		for (auto line : m_PipelineList)
		{
			line->render(camera);
		}
	}

	void PipelineGroup::addMeshRenderer(MeshRenderer* meshRenderer)
	{
		auto material = meshRenderer->getMaterial();
		auto shaders = material->getShaderPackage()->getShaders();
		for (auto shader : shaders)
		{
			auto pass = PipelineManager::getPass(shader->getUID());
			if (pass->isAttached())
			{
				pass->addMeshRenderer(meshRenderer);
			}
			else
			{
				m_PipelineList[shader->getRenderQueue()]->addPass(pass);
			}
		}
	}

}

