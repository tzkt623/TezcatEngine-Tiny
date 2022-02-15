#include "Renderer.h"
#include "Platform/OpenGL/GLHead.h"
#include "../GUI/GUI.h"
#include "../Engine.h"
#include "../Statistic.h"
#include "../Manager/VertexGroupManager.h"
#include "../Manager/CameraManager.h"
#include "../Component/MeshRenderer.h"

#include "Utility/Tools.h"


namespace tezcat::Tiny::Core
{
	Renderer::Renderer()
		: m_GUI(new GUI())
		, m_VAOManager(new VertexGroupManager())
	{
		SG<Renderer>::attach(this);
	}

	Renderer::~Renderer()
	{
		delete m_GUI;
	}

	void Renderer::init(Engine* engine)
	{
		m_GUI->init(engine);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		GLint max;
		glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &max);
		std::cout << "GL_MAX_UNIFORM_LOCATIONS: " << max << std::endl;
	}

	void Renderer::addMeshRenderer(MeshRenderer* renderer)
	{
		m_MeshRendererList.push_back(renderer);
	}

	void Renderer::render()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Statistic::DrawCall = 0;

		for (auto mesh_renderer : m_MeshRendererList)
		{
			if (mesh_renderer->isEnable())
			{
				mesh_renderer->sendDataToGPU();
			}
		}

		SG<CameraManager>::getInstance()->render();

		if (m_GUI)
		{
			m_GUI->render();
		}
	}
}