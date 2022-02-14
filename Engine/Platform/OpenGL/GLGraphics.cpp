#include "GLGraphics.h"
#include "GLVertexBuffer.h"

#include "Core/Head/GLHead.h"
#include "Core/Head/CppHead.h"
#include "Core/Component/MeshRenderer.h"
#include "Core/Engine.h"
#include "Core/Statistic.h"
#include "Core/GUI/GUI.h"
#include "Core/Shader/ShaderPackage.h"
#include "Core/Shader/ShaderBuilder.h"
#include "../Windows/WindowsEngine.h"

namespace tezcat::Tiny::Core
{
	GLGraphics::GLGraphics()
		: m_Window(nullptr)
	{
		
	}

	GLGraphics::~GLGraphics()
	{
		m_Window = nullptr;
	}

	void GLGraphics::init(Engine* engine)
	{
		auto we = (WindowsEngine*)engine;
		m_Window = we->getGLFWWindow();
		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(1);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			throw std::logic_error("Failed to initialize GLAD");
		}

		(new GUI())->init(engine);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		GLint max;
		glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &max);
		std::cout << "GL_MAX_UNIFORM_LOCATIONS: " << max << std::endl;
	}

	void GLGraphics::preRender()
	{
		BaseGraphics::preRender();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLGraphics::onRender()
	{
		BaseGraphics::onRender();
	}

	void GLGraphics::postRender()
	{
		BaseGraphics::postRender();
		SG<GUI>::getInstance()->render();
	}

	void GLGraphics::swapBuffer()
	{
		glfwSwapBuffers(m_Window);
	}

	void GLGraphics::renderMesh(MeshRenderer* renderer)
	{
		if (renderer->hasIndex())
		{
			glDrawElements(renderer->getDrawMode(), renderer->getIndexCount(), GL_UNSIGNED_INT, nullptr);
		}
		else
		{
			glDrawArrays(renderer->getDrawMode(), 0, renderer->getVertexCount());
		}
	}

	VertexBuffer* GLGraphics::createVertexBuffer(MeshData* mesh)
	{
		return new GLVertexBuffer(mesh);
	}

	VertexGroup* GLGraphics::createVertexGroup(MeshData* mesh)
	{
		return new GLVertexGroup(mesh);
	}

	void GLGraphics::createShaderPackage(const std::string& filePath)
	{
		ShaderBuilder::createPackage(filePath);
	}

}
