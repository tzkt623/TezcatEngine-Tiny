#include "GLGraphics.h"
#include "GLVertexBuffer.h"

#include "GLHead.h"
#include "GLShaderBuilder.h"
#include "GLTexture.h"

#include "../Windows/WindowsEngine.h"

#include "Core/Head/CppHead.h"
#include "Core/Component/MeshRenderer.h"
#include "Core/Engine.h"
#include "Core/Statistic.h"
#include "Core/GUI/GUI.h"
#include "Core/Shader/ShaderPackage.h"
#include "Core/Head/ContextMap.h"
#include "Core/Component/Camera.h"

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

		this->initContextMap();

		(new GUI())->init(engine);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		GLint max;
		glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &max);
		std::cout << "GL_MAX_UNIFORM_LOCATIONS: " << max << std::endl;
	}

	void GLGraphics::updateViewport(Camera* camera)
	{
		auto& info = camera->getViewRect();
		glViewport(info.OX, info.OY, info.Width, info.Height);
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
			glDrawElements((GLenum)renderer->getDrawMode(), renderer->getIndexCount(), GL_UNSIGNED_INT, nullptr);
		}
		else
		{
			glDrawArrays((GLenum)renderer->getDrawMode(), 0, renderer->getVertexCount());
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
		GLShaderBuilder::createPackage(filePath);
	}

	Texture* GLGraphics::createTexture(const std::string& filePath, TextureType type)
	{
		Texture* texture = nullptr;
		switch (type)
		{
		case TextureType::Texture1D:
			break;
		case TextureType::Texture2D:
			texture = new GLTexture2D();
			break;
		case TextureType::Texture3D:
			break;
		case TextureType::TextureCube:
			break;
		case TextureType::Texture1DA:
			break;
		case TextureType::Texture2DA:
			break;
		default:
			break;
		}

		texture->createTexture(filePath);
		return texture;
	}

	void GLGraphics::initContextMap()
	{
		ContextMap::DrawMode =
		{
			GL_POINTS,
			GL_LINES,
			GL_LINE_LOOP,
			GL_LINE_STRIP,
			GL_TRIANGLES,
			GL_TRIANGLE_STRIP,
			GL_TRIANGLE_FAN
		};

		ContextMap::TextureType =
		{
			GL_TEXTURE_1D,
			GL_TEXTURE_2D,
			GL_TEXTURE_3D,
			GL_TEXTURE_CUBE_MAP,
			GL_TEXTURE_1D_ARRAY,
			GL_TEXTURE_2D_ARRAY
		};

		ContextMap::TextureFilter =
		{
			GL_NEAREST,
			GL_LINEAR
		};

		ContextMap::TextureWrap =
		{
			GL_REPEAT,
			GL_MIRRORED_REPEAT,
			GL_CLAMP_TO_EDGE,
			GL_CLAMP_TO_BORDER
		};

		ContextMap::TextureChannel =
		{
			GL_RGB,
			GL_RGBA
		};
	}

}
