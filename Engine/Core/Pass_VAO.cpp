#include "Pass_VAO.h"
#include "Pass_Shader.h"
#include "VAO.h"
#include "Statistic.h"
#include "RenderAgent.h"
#include "Shader.h"

namespace tezcat::Tiny::Core
{
	Pass_VAO::Pass_VAO(VAO* vao) :
		m_DelegateVAO(vao)
	{

	}

	Pass_VAO::~Pass_VAO()
	{
	}

	void Pass_VAO::addRenderAgent(RenderAgent* renderAgent)
	{
		m_AgentList.push_back(renderAgent);
	}

	void Pass_VAO::render(Shader* shader)
	{
		// 共用同一个VAO
		m_DelegateVAO->bind();
		Statistic::DrawCall += m_AgentList.size();

		for (auto agent : m_AgentList)
		{
			agent->render(shader);
		}
	}
}