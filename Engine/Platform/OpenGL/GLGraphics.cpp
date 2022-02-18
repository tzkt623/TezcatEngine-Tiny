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
#include "Core/Head/Context.h"
#include "Core/Component/Camera.h"

namespace tezcat::Tiny::Core
{
	GLGraphics::GLGraphics()
		: m_Window(nullptr)
	{
		TextureMgr::init(new GLTextureCreator());
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

		this->initContext();

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
			glDrawElements(renderer->getDrawMode().platform, renderer->getIndexCount(), GL_UNSIGNED_INT, nullptr);
		}
		else
		{
			glDrawArrays(renderer->getDrawMode().platform, 0, renderer->getVertexCount());
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

	void GLGraphics::initContext()
	{
		Statistic::GPU = glGetString(GL_RENDERER);

		ContextMap::DrawModeArray =
		{
			DrawModeWrapper(DrawMode::Points,			GL_POINTS),
			DrawModeWrapper(DrawMode::Lines,			GL_LINES),
			DrawModeWrapper(DrawMode::Lines_Loop,		GL_LINE_LOOP),
			DrawModeWrapper(DrawMode::Lines_Loop,		GL_LINE_STRIP),
			DrawModeWrapper(DrawMode::Triangles,		GL_TRIANGLES),
			DrawModeWrapper(DrawMode::Triangles_Strip,	GL_TRIANGLE_STRIP),
			DrawModeWrapper(DrawMode::Triangles_Fan,	GL_TRIANGLE_FAN)
		};

		ContextMap::TextureTypeArray =
		{
			TexTypeWrapper(TextureType::Texture1D,		GL_TEXTURE_1D),
			TexTypeWrapper(TextureType::Texture2D,		GL_TEXTURE_2D),
			TexTypeWrapper(TextureType::Texture2D,		GL_TEXTURE_3D),
			TexTypeWrapper(TextureType::TextureCube,	GL_TEXTURE_CUBE_MAP),
			TexTypeWrapper(TextureType::Texture1DA,		GL_TEXTURE_1D_ARRAY),
			TexTypeWrapper(TextureType::Texture2DA,		GL_TEXTURE_2D_ARRAY)
		};

		ContextMap::TextureFilterArray =
		{
			TexFilterWrapper(TextureFilter::Tex_NEAREST, GL_NEAREST),
			TexFilterWrapper(TextureFilter::Tex_NEAREST, GL_LINEAR),
		};

		ContextMap::TextureWrapArray =
		{
			TexWrapWrapper(TextureWrap::Tex_REPEAT,				GL_REPEAT),
			TexWrapWrapper(TextureWrap::Tex_MIRRORED_REPEAT,	GL_MIRRORED_REPEAT),
			TexWrapWrapper(TextureWrap::Tex_CLAMP_TO_EDGE,		GL_CLAMP_TO_EDGE),
			TexWrapWrapper(TextureWrap::Tex_CLAMP_TO_BORDER,	GL_CLAMP_TO_BORDER)
		};

		ContextMap::TextureChannelArray =
		{
			TexChannelWrapper(TextureChannel::Tex_RGB,	GL_RGB),
			TexChannelWrapper(TextureChannel::Tex_RGBA, GL_RGBA)
		};

		ContextMap::BlendMap =
		{
			{"0",			BlendWrapper(Blend::Zero,					GL_ZERO)},
			{"1",			BlendWrapper(Blend::One,					GL_ONE)},
			{"Src",			BlendWrapper(Blend::Source,					GL_SRC_COLOR)},
			{"1-Src",		BlendWrapper(Blend::One_Minus_Source,		GL_ONE_MINUS_SRC_COLOR)},
			{"Tar",			BlendWrapper(Blend::Target,					GL_DST_COLOR)},
			{"1-Tar",		BlendWrapper(Blend::One_Minus_Target,		GL_ONE_MINUS_DST_COLOR)},
			{"SrcA",		BlendWrapper(Blend::SourceAlpha,			GL_SRC_ALPHA)},
			{"1-SrcA",		BlendWrapper(Blend::One_Minus_SourceAlpha,	GL_ONE_MINUS_SRC_ALPHA)},
			{"TarA",		BlendWrapper(Blend::TargetAlpha,			GL_DST_ALPHA)},
			{"1-TarA",		BlendWrapper(Blend::One_Minus_Target,		GL_ONE_MINUS_DST_ALPHA)},
			{"Const",		BlendWrapper(Blend::Const,					GL_CONSTANT_COLOR)},
			{"1-Const",		BlendWrapper(Blend::One_Minus_Const,		GL_ONE_MINUS_CONSTANT_COLOR)},
			{"ConstA",		BlendWrapper(Blend::ConstAlpha,				GL_CONSTANT_ALPHA)},
			{"1-ConstA",	BlendWrapper(Blend::One_Minus_ConstAlpha,	GL_ONE_MINUS_CONSTANT_ALPHA)}
		};

		ContextMap::BlendArray =
		{
			BlendWrapper(Blend::Zero,					GL_ZERO),
			BlendWrapper(Blend::One,					GL_ONE),
			BlendWrapper(Blend::Source,					GL_SRC_COLOR),
			BlendWrapper(Blend::One_Minus_Source,		GL_ONE_MINUS_SRC_COLOR),
			BlendWrapper(Blend::Target,					GL_DST_COLOR),
			BlendWrapper(Blend::One_Minus_Target,		GL_ONE_MINUS_DST_COLOR),
			BlendWrapper(Blend::SourceAlpha,			GL_SRC_ALPHA),
			BlendWrapper(Blend::One_Minus_SourceAlpha,	GL_ONE_MINUS_SRC_ALPHA),
			BlendWrapper(Blend::TargetAlpha,			GL_DST_ALPHA),
			BlendWrapper(Blend::One_Minus_TargetAlpha,	GL_ONE_MINUS_DST_ALPHA),
			BlendWrapper(Blend::Const,					GL_CONSTANT_COLOR),
			BlendWrapper(Blend::One_Minus_Const,		GL_ONE_MINUS_CONSTANT_COLOR),
			BlendWrapper(Blend::ConstAlpha,				GL_CONSTANT_ALPHA),
			BlendWrapper(Blend::One_Minus_ConstAlpha,	GL_ONE_MINUS_CONSTANT_ALPHA)
		};

		ContextMap::CullFaceMap =
		{
			{"Off",		CullFaceWrapper(CullFace::Off,		0)},
			{"Front",	CullFaceWrapper(CullFace::Front,	GL_FRONT)},
			{"Back",	CullFaceWrapper(CullFace::Back,		GL_BACK)},
			{"All",		CullFaceWrapper(CullFace::All,		GL_FRONT_AND_BACK)}
		};

		ContextMap::CullFaceArray =
		{
			CullFaceWrapper(CullFace::Off,		0),
			CullFaceWrapper(CullFace::Front,	GL_FRONT),
			CullFaceWrapper(CullFace::Back,		GL_BACK),
			CullFaceWrapper(CullFace::All,		GL_FRONT_AND_BACK),
		};
	}
}