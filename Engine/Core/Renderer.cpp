#include "Renderer.h"
#include "Pass_VAO.h"
#include "Material.h"
#include "Shader.h"
#include "Pass_Shader.h"
#include "RenderObject.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"
#include "CameraManager.h"
#include "GUI.h"
#include "Engine.h"
#include "Statistic.h"
#include "RenderAgent.h"
#include "VAOManager.h"


namespace tezcat::Tiny::Core
{
	IMPLEMENT_DELEGATE_INSTANCE(Renderer);
	Renderer::Renderer() :
		m_GUI(new GUI()),
		m_VAOManager(new VAOManager())
	{
		REGISTER_DELEGATE_INSTANCE(Renderer);
	}

	Renderer::~Renderer()
	{
		delete m_GUI;
	}

	void Renderer::addPass(Pass_Shader* pass)
	{
		if (m_PassList.empty())
		{
			m_PassList.push_front(pass);
			return;
		}

		auto it = m_PassList.begin();
		while (it != m_PassList.end())
		{
			if ((*it)->getOrderID() <= pass->getOrderID())
			{
				m_PassList.insert(it, pass);
				break;
			}
			it++;
		}
	}

	Pass_Shader* Core::Renderer::getPass(int id)
	{
		auto it = m_PassWithID.find(id);
		if (it != m_PassWithID.end())
		{
			return it->second;
		}

		return nullptr;
	}

	Pass_Shader* Core::Renderer::getPass(const std::string& name)
	{
		auto it = m_PassWithName.find(name);
		if (it != m_PassWithName.end())
		{
			return it->second;
		}

		return nullptr;
	}

	void Renderer::render()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Statistic::DrawCall = 0;

		auto& cameras = CameraManager::getInstance()->getAllCamera();
		for (auto camera : cameras)
		{
			auto it = m_PassList.begin();
			while (it != m_PassList.end())
			{
				(*it++)->render(camera);
			}
		}

		if (m_GUI)
		{
			m_GUI->render();
		}
	}

	void Renderer::dispatch(RenderAgent* renderAgent)
	{
		auto shader = renderAgent->getMaterial()->getShader();

		if (m_PassWithID.find(shader->getProgramID()) == m_PassWithID.end())
		{
			auto pass = new Pass_Shader(shader);
			this->addPass(pass);
			pass->addRenderAgent(renderAgent);

			m_PassWithName.emplace(pass->getName(), pass);
			m_PassWithID.emplace(pass->getProgramID(), pass);

			Statistic::PassCount = m_PassWithID.size();
		}
		else
		{
			m_PassWithID[shader->getProgramID()]->addRenderAgent(renderAgent);
		}
	}

	void Renderer::init(Engine* engine)
	{
		m_GUI->init(engine);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

}