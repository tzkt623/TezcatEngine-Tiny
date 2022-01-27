#include "Renderer.h"
#include "PassQueue.h"
#include "glad/glad.h"
#include "Material.h"
#include "Shader.h"
#include "Pass.h"
#include "RenderObject.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"
#include "CameraManager.h"
#include "GUI.h"
#include "Engine.h"
#include "Statistic.h"


namespace tezcat::Tiny::Core
{
	Renderer::Renderer() :
		m_GUI(new GUI())
	{

	}

	Renderer::~Renderer()
	{
		delete m_GUI;
	}

	void Renderer::addPass(Pass* passQueue)
	{
		if (m_PassList.empty())
		{
			m_PassList.push_front(passQueue);
			return;
		}

		auto it = m_PassList.begin();
		while (it != m_PassList.end())
		{
			if ((*it)->getID() < passQueue->getID())
			{
				m_PassList.insert(it, passQueue);
				m_PassWithID.emplace((*it)->getID(), (*it));
				m_PassWithName.emplace((*it)->getName(), (*it));
				break;
			}
			it++;
		}
	}

	Pass* Core::Renderer::getPass(int id)
	{
		auto it = m_PassWithID.find(id);
		if (it != m_PassWithID.end())
		{
			return it->second;
		}

		return nullptr;
	}

	Pass* Core::Renderer::getPass(const std::string& name)
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
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Engine::getInstance()->getCameraManager()->foreach(
			[this](Module::Camera* camera)
			{
				auto it = m_PassList.begin();
				while (it != m_PassList.end())
				{
					(*it++)->render(camera);
				}
			});

		if (m_GUI)
		{
			m_GUI->render();
		}
	}

	void Renderer::dispatch(Module::GameObject* gameObject)
	{
		auto shader = gameObject->getMaterial()->getShader();

		if (m_PassWithID.find(shader->getOrderID()) == m_PassWithID.end())
		{
			auto pass = new Pass(shader);
			this->addPass(pass);
			pass->addRenderObject(gameObject->getRenderObject());

			m_PassWithID.emplace(shader->getOrderID(), pass);
			Statistic::PassCount = m_PassWithID.size();
		}
		else
		{
			m_PassWithID[shader->getOrderID()]->addRenderObject(gameObject->getRenderObject());
		}
	}

	void Renderer::init(Engine* engine)
	{
		m_GUI->init(engine);
	}

}