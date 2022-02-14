#include "PipelineManager.h"
#include "../Shader/Shader.h"
#include "../Pipeline/PassShader.h"

namespace tezcat::Tiny::Core
{
	std::vector<PassShader*> PipelineManager::m_PassList(100);
	void PipelineManager::createPass(Shader* shader)
	{
		while (m_PassList.size() <= shader->getUID())
		{
			m_PassList.push_back(nullptr);
		}

		m_PassList[shader->getUID()] = new PassShader(shader);
	}
}