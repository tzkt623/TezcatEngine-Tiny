#include "Pass_Shader.h"
#include "Shader.h"
#include "RenderObject.h"
#include "Camera.h"
#include "Material.h"
#include "Statistic.h"
#include "RenderAgent.h"
#include "Mesh.h"
#include "VAO.h"
#include "Pass_VAO.h"

namespace tezcat::Tiny::Core
{
	Pass_Shader::Pass_Shader(Shader* program) :
		m_Shader(program)
	{

	}

	Pass_Shader::Pass_Shader(Module::Material* material)
	{
		m_Shader = material->getShader();
	}

	Pass_Shader::~Pass_Shader()
	{
		m_Shader = nullptr;
	}

	void Pass_Shader::render(Module::Camera* camera)
	{
		// 共用同一个shader
		m_Shader->bind();
		m_Shader->setProjectionMatrix(camera->getProjectionMatrix());
		m_Shader->setViewMatrix(camera->getViewMatrix());

		auto it = m_AgentWithID.begin();
		while (it != m_AgentWithID.end())
		{
			auto vao_pass = (*it++).second;
			if (!vao_pass->empty())
			{
				vao_pass->render(m_Shader);
			}
		}
	}

	int Pass_Shader::getOrderID() const
	{
		return m_Shader->getOrderID();
	}

	int Pass_Shader::getProgramID() const
	{
		return m_Shader->getProgramID();
	}

	const std::string& Pass_Shader::getName() const
	{
		return m_Shader->getName();
	}

	void Pass_Shader::addRenderAgent(RenderAgent* renderAgent)
	{
		auto vao = renderAgent->getMesh()->getVAO();
		if (m_AgentWithID.find(vao) == m_AgentWithID.end())
		{
			m_AgentWithID.emplace(vao, new Pass_VAO(vao));
		}

		m_AgentWithID[vao]->addRenderAgent(renderAgent);
	}
}
