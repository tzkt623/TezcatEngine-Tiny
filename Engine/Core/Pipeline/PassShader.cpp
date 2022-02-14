#include "PassShader.h"
#include "PassVertexGroup.h"

#include "../Shader/Shader.h"
#include "../Component/Camera.h"
#include "../Component/Transform.h"
#include "../Component/MeshRenderer.h"
#include "../Head/GLMHead.h"


namespace tezcat::Tiny::Core
{
	PassShader::PassShader(Shader* program)
		: m_Shader(program)
		, m_InPipeline(false)
	{

	}

	PassShader::~PassShader()
	{
		m_Shader = nullptr;
	}

	void PassShader::addMeshRenderer(MeshRenderer* meshRenderer)
	{
		auto vao = meshRenderer->getVertexGroup();
		auto it = m_VAOWithID.find(vao);
		if (it == m_VAOWithID.end())
		{
			auto pass = new PassVertexGroup(vao);
			pass->addMeshRenderer(meshRenderer);
			m_VAOWithID.emplace(vao, pass);
		}
		else
		{
			it->second->addMeshRenderer(meshRenderer);
		}
	}

	void PassShader::render(Camera* camera)
	{
		m_Shader->setGPUOptions();
		// 共用同一个shader
		m_Shader->bind();

		if (m_Shader->isEnableLighting())
		{
			// 			auto light_data = SG<LightManager>::getInstance()->getData();
			// 			light_data->render();
			// 			for (auto light : lights)
			// 			{
			// 				light->render(m_Shader);
			// 			}
		}

		m_Shader->setProjectionMatrix(camera->getProjectionMatrix());
		m_Shader->setViewMatrix(camera->getViewMatrix());
		m_Shader->setViewPosition(camera->getTransform()->getPosition());

		auto it = m_VAOWithID.begin();
		while (it != m_VAOWithID.end())
		{
			auto vao_pass = (*it++).second;
			if (!vao_pass->empty())
			{
				vao_pass->render(m_Shader);
			}
		}
	}

	inline int PassShader::getOrderID() const
	{
		return m_Shader->getOrderID();
	}

	inline int PassShader::getProgramID() const
	{
		return m_Shader->getProgramID();
	}

	inline const std::string& PassShader::getName() const
	{
		return m_Shader->getName();
	}
}