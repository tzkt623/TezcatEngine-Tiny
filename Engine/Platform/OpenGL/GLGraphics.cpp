#include "GLGraphics.h"
#include "GLVertexBuffer.h"

#include "GLHead.h"
#include "GLShaderBuilder.h"
#include "GLTexture.h"

#include "WindowsEngine.h"

#include "Core/Head/CppHead.h"
#include "Core/Component/MeshRenderer.h"
#include "Core/Engine.h"
#include "Core/Statistic.h"
#include "Core/GUI/GUI.h"
#include "Core/Shader/ShaderPackage.h"
#include "Core/Head/Context.h"
#include "Core/Component/Camera.h"

namespace tezcat::Tiny::GL
{
	GLGraphics::GLGraphics()
		: mWindow(nullptr)
	{
		TextureMgr::init(new GLTextureCreator());
		VertexBufferCreator::attach(new GLVertexBufferCreator());
		VertexGroupCreator::attach(new GLVertexGroupCreator());
		ShaderBuilderCreator::attach(new GLShaderBuilderCreator());
	}

	GLGraphics::~GLGraphics()
	{
		VertexBufferCreator::destroy();
		VertexGroupCreator::destroy();
		mWindow = nullptr;
	}

	void GLGraphics::init(Engine* engine)
	{
		auto we = (WindowsEngine*)engine;
		mWindow = we->getGLFWWindow();
		glfwMakeContextCurrent(mWindow);
		glfwSwapInterval(1);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			throw std::logic_error("Failed to initialize GLAD");
		}

		this->initContext();

		(new GUI())->init(engine);

// 		glEnable(GL_DEPTH_TEST);
// 		glEnable(GL_CULL_FACE);
// 		glCullFace(GL_BACK);

		GLint max;
		glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &max);
		std::cout << "GL_MAX_UNIFORM_LOCATIONS: " << max << std::endl;
	}

	void GLGraphics::updateViewport(ViewportInfo& info)
	{
		glViewport(info.OX, info.OY, info.Width, info.Height);
	}

	void GLGraphics::preRender()
	{
		BaseGraphics::preRender();
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
		glfwSwapBuffers(mWindow);
	}

	void GLGraphics::clear()
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLGraphics::draw(MeshRenderer* renderer)
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

	void GLGraphics::draw(VertexGroup* group, DrawModeWrapper drawMode)
	{
		if (group->getIndexCount() > 0)
		{
			glDrawElements(drawMode.platform, group->getIndexCount(), GL_UNSIGNED_INT, nullptr);
		}
		else
		{
			glDrawArrays(drawMode.platform, 0, group->getVertexCount());
		}
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
//		std::cout << ContextMap::BlendMap.size() << std::endl;

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

		ContextMap::DepthTestMap =
		{
			{"Off",				DepthTestWrapper(DepthTest::Off,			0)},
			{"Always",			DepthTestWrapper(DepthTest::Always,			GL_ALWAYS)},
			{"Never",			DepthTestWrapper(DepthTest::Never,			GL_NEVER)},
			{"Less",			DepthTestWrapper(DepthTest::Less,			GL_LESS)},
			{"LessEqual",		DepthTestWrapper(DepthTest::LessEqual,		GL_LEQUAL)},
			{"Greater",			DepthTestWrapper(DepthTest::Greater,		GL_GREATER)},
			{"GreaterEqual",	DepthTestWrapper(DepthTest::GreaterEqual,	GL_GEQUAL)},
			{"Equal",			DepthTestWrapper(DepthTest::Equal,			GL_EQUAL)},
			{"NotEqual",		DepthTestWrapper(DepthTest::NotEqual,		GL_NOTEQUAL)}
		};

		ContextMap::DepthTestArray =
		{
			DepthTestWrapper(DepthTest::Off,			0),
			DepthTestWrapper(DepthTest::Always,			GL_ALWAYS),
			DepthTestWrapper(DepthTest::Never,			GL_NEVER),
			DepthTestWrapper(DepthTest::Less,			GL_LESS),
			DepthTestWrapper(DepthTest::LessEqual,		GL_LEQUAL),
			DepthTestWrapper(DepthTest::Greater,		GL_GREATER),
			DepthTestWrapper(DepthTest::GreaterEqual,	GL_GEQUAL),
			DepthTestWrapper(DepthTest::Equal,			GL_EQUAL),
			DepthTestWrapper(DepthTest::NotEqual,		GL_NOTEQUAL)
		};
	}
}
