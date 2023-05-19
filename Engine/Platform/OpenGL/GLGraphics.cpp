#include "GLGraphics.h"
#include "GLBuffer.h"

#include "GLHead.h"
#include "GLShaderBuilder.h"
#include "GLTexture.h"
#include "GLFrameBuffer.h"

#include "WindowsEngine.h"

#include "Core/Head/CppHead.h"
#include "Core/Head/RenderConfig.h"

#include "Core/Component/MeshRenderer.h"
#include "Core/Component/Camera.h"

#include "Core/Engine.h"
#include "Core/Profiler.h"
#include "Core/GUI/GUI.h"
#include "Core/Shader/ShaderPackage.h"
#include "Core/Renderer/RenderObject.h"

namespace tezcat::Tiny::GL
{
	GLGraphics::GLGraphics()
	{
		Graphics::attach(this);

		FrameBufferMgr::getInstance()->initCreator(new GLFrameBufferCreator());
		TextureMgr::getInstance()->initCreator(new GLTextureCreator());
		VertexBufMgr::getInstance()->initCreator(new GLBufferCreator());
		ShaderMgr::getInstance()->initCreator(new GLShaderCreator());
	}

	GLGraphics::~GLGraphics()
	{

	}

	void GLGraphics::init(Engine* engine)
	{
		this->initContext();
		BaseGraphics::init(engine);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	}

	void GLGraphics::initContext()
	{
		Profiler::GPU = glGetString(GL_RENDERER);

		ContextMap::DataTypeArray =
		{
			DataTypeWrapper(DataType::Byte,			GL_BYTE),
			DataTypeWrapper(DataType::UByte,		GL_UNSIGNED_BYTE),
			DataTypeWrapper(DataType::Short,		GL_SHORT),
			DataTypeWrapper(DataType::UShort,		GL_UNSIGNED_SHORT),
			DataTypeWrapper(DataType::Int32,		GL_INT),
			DataTypeWrapper(DataType::UInt32,		GL_UNSIGNED_INT),
			DataTypeWrapper(DataType::Int64,		GL_INT),
			DataTypeWrapper(DataType::UInt64,		GL_UNSIGNED_INT),
			DataTypeWrapper(DataType::Float32,		GL_FLOAT),
			DataTypeWrapper(DataType::Float64,		GL_DOUBLE),
			DataTypeWrapper(DataType::UInt_24_8,	GL_UNSIGNED_INT_24_8),
		};

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
			TexTypeWrapper(TextureType::Texture1D,			GL_TEXTURE_1D),
			TexTypeWrapper(TextureType::Texture2D,			GL_TEXTURE_2D),
			TexTypeWrapper(TextureType::Texture3D,			GL_TEXTURE_3D),
			TexTypeWrapper(TextureType::TextureCube,		GL_TEXTURE_CUBE_MAP),
			TexTypeWrapper(TextureType::Texture1DA,			GL_TEXTURE_1D_ARRAY),
			TexTypeWrapper(TextureType::Texture2DA,			GL_TEXTURE_2D_ARRAY),
			TexTypeWrapper(TextureType::TextureRender2D,	GL_RENDERBUFFER),
		};

		ContextMap::TextureFilterArray =
		{
			TexFilterWrapper(TextureFilter::Nearest,				GL_NEAREST),
			TexFilterWrapper(TextureFilter::Linear,					GL_LINEAR),
			TexFilterWrapper(TextureFilter::Linear_Mipmap_Linear,	GL_LINEAR_MIPMAP_LINEAR),
		};

		ContextMap::TextureWrapArray =
		{
			TexWrapWrapper(TextureWrap::Repeat,				GL_REPEAT),
			TexWrapWrapper(TextureWrap::Mirrored_Repeat,	GL_MIRRORED_REPEAT),
			TexWrapWrapper(TextureWrap::Clamp_To_Edge,		GL_CLAMP_TO_EDGE),
			TexWrapWrapper(TextureWrap::Clamp_To_Border,	GL_CLAMP_TO_BORDER)
		};

		ContextMap::TextureChannelArray =
		{
			TexChannelWrapper(TextureChannel::None,	0),
			TexChannelWrapper(TextureChannel::R,					GL_RED),
			TexChannelWrapper(TextureChannel::RG,					GL_RG),
			TexChannelWrapper(TextureChannel::RGB,					GL_RGB),
			TexChannelWrapper(TextureChannel::RGBA,					GL_RGBA),
			TexChannelWrapper(TextureChannel::Depth,				GL_DEPTH_COMPONENT),
			TexChannelWrapper(TextureChannel::Depth16,				GL_DEPTH_COMPONENT16),
			TexChannelWrapper(TextureChannel::Depth24,				GL_DEPTH_COMPONENT24),
			TexChannelWrapper(TextureChannel::Depth32,				GL_DEPTH_COMPONENT32),
			TexChannelWrapper(TextureChannel::Depth32f,				GL_DEPTH_COMPONENT32F),
			TexChannelWrapper(TextureChannel::Depth_Stencil,		GL_DEPTH_STENCIL),
			TexChannelWrapper(TextureChannel::Depth24_Stencil8,		GL_DEPTH24_STENCIL8),
			TexChannelWrapper(TextureChannel::Depth32f_Stencil8,	GL_DEPTH32F_STENCIL8),
			TexChannelWrapper(TextureChannel::Stencil8,				GL_STENCIL_INDEX8),
			TexChannelWrapper(TextureChannel::RG16f,				GL_RG16F),
			TexChannelWrapper(TextureChannel::RGB16f,				GL_RGB16F),
		};

		ContextMap::ColorBufferArray =
		{
			ColorBufferWrapper(ColorBuffer::None,	GL_NONE),
			ColorBufferWrapper(ColorBuffer::A0,		GL_FRONT_LEFT),
			ColorBufferWrapper(ColorBuffer::A1,		GL_FRONT_RIGHT),
			ColorBufferWrapper(ColorBuffer::B0,		GL_BACK_LEFT),
			ColorBufferWrapper(ColorBuffer::B1,		GL_BACK_RIGHT),
			ColorBufferWrapper(ColorBuffer::AX,		GL_FRONT),
			ColorBufferWrapper(ColorBuffer::BX,		GL_BACK),
			ColorBufferWrapper(ColorBuffer::X0,		GL_LEFT),
			ColorBufferWrapper(ColorBuffer::X1,		GL_RIGHT),
			ColorBufferWrapper(ColorBuffer::All,	GL_FRONT_AND_BACK)
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

	void GLGraphics::setViewport(const ViewportInfo& info)
	{
		glViewport(info.OX, info.OY, info.Width, info.Height);
	}

	void GLGraphics::preRender()
	{
		BaseGraphics::preRender();
	}

	void GLGraphics::onRender()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		BaseGraphics::onRender();
	}

	void GLGraphics::postRender()
	{
		BaseGraphics::postRender();
	}

	void GLGraphics::clear(const ClearOption& option)
	{
		if (option == ClearOption::CO_None)
		{
			return;
		}

		GLbitfield mask = 0;
		if ((option & ClearOption::CO_Color) == ClearOption::CO_Color)
		{
			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			mask |= GL_COLOR_BUFFER_BIT;
		}

		if ((option & ClearOption::CO_Depth) == ClearOption::CO_Depth)
		{
			//如果深度缓冲不允许写入
			//那么也无法clear
			//所以这里必须要打开
			glDepthMask(GL_TRUE);
			glClearDepth(1.0);
			mask |= GL_DEPTH_BUFFER_BIT;
		}

		if ((option & ClearOption::CO_Stencil) == ClearOption::CO_Stencil)
		{
			glStencilMask(0xFF);
			mask |= GL_STENCIL_BUFFER_BIT;
		}

		glClear(mask);
	}

	void GLGraphics::draw(Vertex* vertex)
	{
		vertex->bind();
		if (vertex->getIndexCount() > 0)
		{
			glDrawElements(vertex->getDrawMode().platform, vertex->getIndexCount(), GL_UNSIGNED_INT, nullptr);
		}
		else
		{
			glDrawArrays(vertex->getDrawMode().platform, 0, vertex->getVertexCount());
		}
	}

	void GLGraphics::drawLine(Vertex* vertex, const uint32_t& needCount)
	{
		vertex->bind();
		glDrawArrays(GL_LINE, 0, needCount);
	}

	void GLGraphics::drawLine(const glm::vec3& begin, const glm::vec3& end, const glm::vec3& color /*= glm::vec3(0.0f, 1.0f, 0.0f)*/)
	{

	}

}
