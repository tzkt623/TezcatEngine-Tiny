#include "GLGraphics.h"

#include "GLHead.h"
#include "GLShaderBuilder.h"
#include "GLBuildCommand.h"
#include "GLRenderCommand.h"

#include "Core/Head/CppHead.h"
#include "Core/Head/RenderConfig.h"

#include "Core/Component/MeshRenderer.h"
#include "Core/Component/Camera.h"

#include "Core/Engine.h"
#include "Core/Profiler.h"
#include "Core/Shader/ShaderPackage.h"
#include "Core/Shader/Shader.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Texture.h"
#include "Core/Renderer/FrameBuffer.h"
#include "Core/EngineIniter.h"

namespace tezcat::Tiny::GL
{
	GLGraphics::GLGraphics()
		: mWindow(nullptr)
	{
		Graphics::attach(this);
	}

	GLGraphics::~GLGraphics()
	{

	}

	void GLGraphics::init(Engine* engine)
	{
		auto [min, maj] = engine->getResourceLoader()->getGLVersion();

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, maj);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, min);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		mWindow = glfwCreateWindow(engine->getScreenWidth(), engine->getScreenHeight()
			, engine->getResourceLoader()->getName().c_str(), nullptr, nullptr);
		if (mWindow == nullptr)
		{
			//std::cout << "Failed to create GLFW window" << std::endl;
			Log_Error("Failed to create GLFW window");
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(mWindow);
		glfwSwapInterval(engine->getResourceLoader()->isEnabelVsync() ? 1 : 0);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			Log_Error("Failed to initialize GLAD");
			TINY_THROW_LOGIC("Failed to initialize GLAD");
		}

		GLint max;
		glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &max);
		//std::cout << "GL_MAX_UNIFORM_LOCATIONS: " << max << std::endl;
		Log_Engine(StringTool::stringFormat("GL_MAX_UNIFORM_LOCATIONS: %d", max));

		auto get_default_color_buffer = [&](GLint id)
			{
				switch (id)
				{
				case GL_NONE:
					//std::cout << "Default ColorBuffer: GL_NONE" << std::endl;
					Log_Engine("Default ColorBuffer: GL_NONE");
					break;
				case GL_FRONT_LEFT:
					//std::cout << "Default ColorBuffer: GL_FRONT_LEFT" << std::endl;
					Log_Engine("Default ColorBuffer: GL_FRONT_LEFT");
					break;
				case GL_FRONT_RIGHT:
					//std::cout << "Default ColorBuffer: GL_FRONT_RIGHT" << std::endl;
					Log_Engine("Default ColorBuffer: GL_FRONT_RIGHT");
					break;
				case GL_BACK_LEFT:
					//std::cout << "Default ColorBuffer: GL_BACK_LEFT" << std::endl;
					Log_Engine("Default ColorBuffer: GL_BACK_LEFT");
					break;
				case GL_BACK_RIGHT:
					//std::cout << "Default ColorBuffer: GL_BACK_RIGHT" << std::endl;
					Log_Engine("Default ColorBuffer: GL_BACK_RIGHT");
					break;
				case GL_FRONT:
					//std::cout << "Default ColorBuffer: GL_FRONT" << std::endl;
					Log_Engine("Default ColorBuffer: GL_FRONT");
					break;
				case GL_BACK:
					//std::cout << "Default ColorBuffer: GL_BACK" << std::endl;
					Log_Engine("Default ColorBuffer: GL_BACK");
					break;
				case GL_LEFT:
					//std::cout << "Default ColorBuffer: GL_LEFT" << std::endl;
					Log_Engine("Default ColorBuffer: GL_LEFT");
					break;
				case GL_RIGHT:
					//std::cout << "Default ColorBuffer: GL_RIGHT" << std::endl;
					Log_Engine("Default ColorBuffer: GL_RIGHT");
					break;
				case GL_FRONT_AND_BACK:
					//std::cout << "Default ColorBuffer: GL_FRONT_AND_BACK" << std::endl;
					Log_Engine("Default ColorBuffer: GL_FRONT_AND_BACK");
					break;
				default:
					break;
				}
			};

		glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &max);
		//std::cout << "GL_MAX_UNIFORM_LOCATIONS: " << max << std::endl;
		Log_Engine(StringTool::stringFormat("GL_MAX_UNIFORM_LOCATIONS: %d", max));

		glGetIntegerv(GL_DRAW_BUFFER, &max);
		get_default_color_buffer(max);
		glGetIntegerv(GL_READ_BUFFER, &max);
		get_default_color_buffer(max);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		this->initContext();
		BaseGraphics::init(engine);
	}

	void GLGraphics::initContext()
	{
		Profiler::GPU = glGetString(GL_RENDERER);

		ContextMap::DataMemFormatArray =
		{
			DataMemFormatWrapper(DataMemFormat::Byte,		GL_BYTE),
			DataMemFormatWrapper(DataMemFormat::UByte,		GL_UNSIGNED_BYTE),
			DataMemFormatWrapper(DataMemFormat::Short,		GL_SHORT),
			DataMemFormatWrapper(DataMemFormat::UShort,		GL_UNSIGNED_SHORT),
			DataMemFormatWrapper(DataMemFormat::Int32,		GL_INT),
			DataMemFormatWrapper(DataMemFormat::UInt32,		GL_UNSIGNED_INT),
			DataMemFormatWrapper(DataMemFormat::Int64,		GL_INT),
			DataMemFormatWrapper(DataMemFormat::UInt64,		GL_UNSIGNED_INT),
			DataMemFormatWrapper(DataMemFormat::Float,		GL_FLOAT),
			DataMemFormatWrapper(DataMemFormat::Float64,	GL_DOUBLE),
			DataMemFormatWrapper(DataMemFormat::UInt_24_8,	GL_UNSIGNED_INT_24_8),
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

		ContextMap::TextureInternalFormatArray =
		{
			TexInternalFormatWrapper(TextureInternalFormat::None,					GL_NONE),
			TexInternalFormatWrapper(TextureInternalFormat::Depth,					GL_DEPTH_COMPONENT),
			TexInternalFormatWrapper(TextureInternalFormat::Depth_Stencil,			GL_DEPTH_STENCIL),
			TexInternalFormatWrapper(TextureInternalFormat::R,						GL_RED),
			TexInternalFormatWrapper(TextureInternalFormat::RG,						GL_RG),
			TexInternalFormatWrapper(TextureInternalFormat::RGB,					GL_RGB),
			TexInternalFormatWrapper(TextureInternalFormat::RGBA,					GL_RGBA),
			TexInternalFormatWrapper(TextureInternalFormat::R8,						GL_R8),
			TexInternalFormatWrapper(TextureInternalFormat::RG8,					GL_RG8),
			TexInternalFormatWrapper(TextureInternalFormat::RGB8,					GL_RGB8),
			TexInternalFormatWrapper(TextureInternalFormat::RGBA8,					GL_RGBA8),
			TexInternalFormatWrapper(TextureInternalFormat::R16,					GL_R16),
			TexInternalFormatWrapper(TextureInternalFormat::RG16,					GL_RG16),
			TexInternalFormatWrapper(TextureInternalFormat::RGB16,					GL_RGB16),
			TexInternalFormatWrapper(TextureInternalFormat::RGBA16,					GL_RGBA16),
			TexInternalFormatWrapper(TextureInternalFormat::R16F,					GL_R16F),
			TexInternalFormatWrapper(TextureInternalFormat::RG16F,					GL_RG16F),
			TexInternalFormatWrapper(TextureInternalFormat::RGB16F,					GL_RGB16F),
			TexInternalFormatWrapper(TextureInternalFormat::RGBA16F,				GL_RGBA16F),
			TexInternalFormatWrapper(TextureInternalFormat::R32F,					GL_R32F),
			TexInternalFormatWrapper(TextureInternalFormat::RG32F,					GL_RG32F),
			TexInternalFormatWrapper(TextureInternalFormat::RGB32F,					GL_RGB32F),
			TexInternalFormatWrapper(TextureInternalFormat::RGBA32F,				GL_RGBA32F),
		};

		ContextMap::TextureFormatArray =
		{
			TexFormatWrapper(TextureFormat::None,				GL_NONE),
			TexFormatWrapper(TextureFormat::Depth,				GL_DEPTH_COMPONENT),
			TexFormatWrapper(TextureFormat::Stencil,			GL_STENCIL_INDEX),
			TexFormatWrapper(TextureFormat::Depth_Stencil,		GL_DEPTH_STENCIL),
			TexFormatWrapper(TextureFormat::R,					GL_RED),
			TexFormatWrapper(TextureFormat::RG,					GL_RG),
			TexFormatWrapper(TextureFormat::RGB,				GL_RGB),
			TexFormatWrapper(TextureFormat::RGBA,				GL_RGBA),
			TexFormatWrapper(TextureFormat::BGR,				GL_BGR),
			TexFormatWrapper(TextureFormat::BGRA,				GL_BGRA),
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

	void GLGraphics::setPassState(Shader* shader)
	{
		if (shader->getCullFaceWrapper().platform != 0)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(shader->getCullFaceWrapper().platform);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}

		if (shader->getDepthTestWrapper().platform != 0)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(shader->getDepthTestWrapper().platform);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}

		//只有在深度测试启用的情况下才有用
		glDepthMask(shader->isEnableZWrite() ? GL_TRUE : GL_FALSE);

		if (shader->isEnableBlend())
		{
			glEnable(GL_BLEND);
			glBlendFunc(shader->getBlendSourceWrapper().platform, shader->getBlendTargetWrapper().platform);
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}


	void GLGraphics::bind(Shader* shader)
	{
		glUseProgram(shader->getProgramID());
	}

	void GLGraphics::bind(FrameBuffer* frameBuffer)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->getFrameBufferID());
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
			//glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
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
		TINY_GL_CHECK(glBindVertexArray(vertex->getVertexID()));
		//glDrawArrays(vertex->getDrawMode().platform, 0, vertex->getVertexCount());

		if (vertex->getIndexCount() > 0)
		{
			TINY_GL_CHECK(glDrawElements(vertex->getDrawMode().platform, vertex->getIndexCount(), GL_UNSIGNED_INT, nullptr));
		}
		else
		{
			TINY_GL_CHECK(glDrawArrays(vertex->getDrawMode().platform, 0, vertex->getVertexCount()));
		}
	}

	void GLGraphics::drawLine(Vertex* vertex, const uint32& needCount)
	{
		glBindVertexArray(vertex->getVertexID());
		glDrawArrays(GL_LINE, 0, needCount);
	}

	void GLGraphics::drawLine(const glm::vec3& begin, const glm::vec3& end, const glm::vec3& color /*= glm::vec3(0.0f, 1.0f, 0.0f)*/)
	{

	}

#pragma region Create
	void GLGraphics::cmdCreateVertex(Vertex* vertex)
	{
		this->createCMD<GLBuildCMD_CreateVertex>(vertex);
	}

	void GLGraphics::cmdCreateVertexBuffer(VertexBuffer* vertexBuffer)
	{
		this->createCMD<GLBuildCMD_CreateVertexBuffer>(vertexBuffer);
	}

	void GLGraphics::cmdCreateIndexBuffer(IndexBuffer* indexBuffer)
	{
		this->createCMD<GLBuildCMD_CreateIndexBuffer>(indexBuffer);
	}

	void GLGraphics::cmdCreateTexture2D(Texture2D* tex2d)
	{
		this->createCMD<GLBuildCMD_CreateTextrue2D>(tex2d);
	}

	void GLGraphics::cmdCreateTextureCube(TextureCube* texCube)
	{
		this->createCMD<GLBuildCMD_CreateTextrueCube>(texCube);
	}

	void GLGraphics::cmdCreateRender2D(TextureRender2D* render2d)
	{
		this->createCMD<GLBuildCMD_CreateRender2D>(render2d);
	}

	void GLGraphics::cmdCreateFrameBuffer(FrameBuffer* frameBuffer)
	{
		this->createCMD<GLBuildCMD_CreateFrameBuffer>(frameBuffer);
	}

	void GLGraphics::cmdCreateShader(Shader* shader)
	{
		this->createCMD<GLBuildCMD_CreateShader>(shader);
	}

	void GLGraphics::cmdCreateShader(Shader* shader, std::string& data)
	{
		this->createCMD<GLBuildCMD_CreateShader>(shader, data);
	}
#pragma endregion

#pragma region Update
	void GLGraphics::cmdUpdateTexture2D(Texture2D* tex2d)
	{
		this->createCMD<GLBuildCMD_UpdateTextrue2D>(tex2d);
	}
#pragma endregion

#pragma region Delete
	void GLGraphics::cmdDeleteTexture2D(uint32 id)
	{
		this->createCMD<GLBuildCMD_DeleteTexture>(id);
	}

	void GLGraphics::cmdDeleteTextureCube(uint32 id)
	{
		this->createCMD<GLBuildCMD_DeleteTexture>(id);
	}

	void GLGraphics::cmdDeleteRender2D(uint32 id)
	{
		this->createCMD<GLBuildCMD_DeleteRender2D>(id);
	}

	void GLGraphics::cmdDeleteVertex(uint32 id)
	{
		this->createCMD<GLBuildCMD_DeleteVAO>(id);
	}

	void GLGraphics::cmdDeleteVertexBuffer(uint32 id)
	{
		this->createCMD<GLBuildCMD_DeleteVBO>(id);
	}

	void GLGraphics::cmdDeleteIndexBuffer(uint32 id)
	{
		this->createCMD<GLBuildCMD_DeleteIBO>(id);
	}

	void GLGraphics::cmdDeleteFrameBuffer(uint32 id)
	{
		this->createCMD<GLBuildCMD_DeleteFrameBuffer>(id);
	}

	void GLGraphics::cmdDeleteShader(uint32 id)
	{
		this->createCMD<GLBuildCMD_DeleteShader>(id);
	}
#pragma endregion

	//-----------------------------------------------------------------
	//
	//	Update Data
	//
	void GLGraphics::setGlobalTexture2D(Shader* shader, UniformID& uniform, Texture2D* data)
	{
		int shader_id;
		if (!shader->checkTinyUniform(uniform, shader_id))
		{
			return;
		}

		glUniform1i(shader_id, shader->getTextureIndex());
		glActiveTexture(GL_TEXTURE0 + shader->getTextureIndex());
		glBindTexture(GL_TEXTURE_2D, data->getTextureID());
		shader->addGlobalTextureIndex();
	}

	void GLGraphics::setGlobalTexture2D(Shader* shader, const int& shaderID, Texture2D* data)
	{
		glUniform1i(shaderID, shader->getTextureIndex());
		glActiveTexture(GL_TEXTURE0 + shader->getTextureIndex());
		glBindTexture(GL_TEXTURE_2D, data->getTextureID());
		shader->addGlobalTextureIndex();
	}

	void GLGraphics::setTexture2D(Shader* shader, UniformID& uniform, Texture2D* data)
	{
		int shader_id;
		if (!shader->checkTinyUniform(uniform, shader_id))
		{
			return;
		}

		glUniform1i(shader_id, shader->getTextureIndex());
		glActiveTexture(GL_TEXTURE0 + shader->getTextureIndex());
		glBindTexture(GL_TEXTURE_2D, data->getTextureID());
		shader->addLocalTextureIndex();
	}

	void GLGraphics::setTexture2D(Shader* shader, const int& shaderID, Texture2D* data)
	{
		glUniform1i(shaderID, shader->getTextureIndex());
		glActiveTexture(GL_TEXTURE0 + shader->getTextureIndex());
		glBindTexture(GL_TEXTURE_2D, data->getTextureID());
		shader->addLocalTextureIndex();
	}

	void GLGraphics::setGlobalTextureCube(Shader* shader, UniformID& uniform, TextureCube* data)
	{
		int shader_id;
		if (!shader->checkTinyUniform(uniform, shader_id))
		{
			return;
		}

		glUniform1i(shader_id, shader->getTextureIndex());
		glActiveTexture(GL_TEXTURE0 + shader->getTextureIndex());
		glBindTexture(GL_TEXTURE_CUBE_MAP, data->getTextureID());
		shader->addGlobalTextureIndex();
	}

	void GLGraphics::setGlobalTextureCube(Shader* shader, const int& shaderID, TextureCube* data)
	{
		glUniform1i(shaderID, shader->getTextureIndex());
		glActiveTexture(GL_TEXTURE0 + shader->getTextureIndex());
		glBindTexture(GL_TEXTURE_CUBE_MAP, data->getTextureID());
		shader->addGlobalTextureIndex();
	}

	void GLGraphics::setTextureCube(Shader* shader, UniformID& uniform, TextureCube* data)
	{
		int shader_id;
		if (!shader->checkTinyUniform(uniform, shader_id))
		{
			return;
		}

		glUniform1i(shader_id, shader->getTextureIndex());
		glActiveTexture(GL_TEXTURE0 + shader->getTextureIndex());
		glBindTexture(GL_TEXTURE_CUBE_MAP, data->getTextureID());
		shader->addLocalTextureIndex();
	}

	void GLGraphics::setTextureCube(Shader* shader, const int& shaderID, TextureCube* data)
	{
		glUniform1i(shaderID, shader->getTextureIndex());
		glActiveTexture(GL_TEXTURE0 + shader->getTextureIndex());
		glBindTexture(GL_TEXTURE_CUBE_MAP, data->getTextureID());
		shader->addLocalTextureIndex();
	}

	void GLGraphics::setBool(Shader* shader, const char* name, const bool& data)
	{
		auto id = glGetUniformLocation(shader->getProgramID(), name);
		glUniform1i(id, data);
	}


	void GLGraphics::setFloat1(Shader* shader, UniformID& uniform, float* data)
	{
		int shader_id;
		if (!shader->checkTinyUniform(uniform, shader_id))
		{
			return;
		}
		glUniform1fv(shader_id, 1, data);
	}

	void GLGraphics::setFloat1(Shader* shader, const char* name, float* data)
	{
		auto id = TINY_GL_CHECK(glGetUniformLocation(shader->getProgramID(), name));
		TINY_GL_CHECK(glUniform1fv(id, 1, data));
	}

	void GLGraphics::setFloat1(Shader* shader, const int& shaderID, const float* data)
	{
		TINY_GL_CHECK(glUniform1fv(shaderID, 1, data));
	}

	void GLGraphics::setFloat1(Shader* shader, const char* name, const float& data)
	{
		auto id = TINY_GL_CHECK(glGetUniformLocation(shader->getProgramID(), name));
		TINY_GL_CHECK(glUniform1f(id, data));
	}

	void GLGraphics::setFloat1(Shader* shader, const int& shaderID, const float& data)
	{
		glUniform1f(shaderID, data);
	}

	void GLGraphics::setFloat2(Shader* shader, UniformID& uniform, float* data)
	{
		int shader_id;
		if (!shader->checkTinyUniform(uniform, shader_id))
		{
			return;
		}
		glUniform2fv(shader_id, 1, data);
	}

	void GLGraphics::setFloat2(Shader* shader, UniformID& uniform, const glm::vec2& data)
	{
		int shader_id;
		if (!shader->checkTinyUniform(uniform, shader_id))
		{
			return;
		}
		glUniform2f(shader_id, data.x, data.y);
	}

	void GLGraphics::setFloat2(Shader* shader, const char* name, float* data)
	{
		auto id = glGetUniformLocation(shader->getProgramID(), name);
		glUniform2fv(id, 1, data);
	}

	void GLGraphics::setFloat2(Shader* shader, const int& shaderID, const float* data)
	{
		glUniform2fv(shaderID, 1, data);
	}

	void GLGraphics::setFloat3(Shader* shader, UniformID& uniform, float* data)
	{
		int shader_id;
		if (!shader->checkTinyUniform(uniform, shader_id))
		{
			return;
		}
		glUniform3fv(shader_id, 1, data);
	}

	void GLGraphics::setFloat3(Shader* shader, UniformID& uniform, const glm::vec3& data)
	{
		int shader_id;
		if (!shader->checkTinyUniform(uniform, shader_id))
		{
			return;
		}
		glUniform3f(shader_id, data.x, data.y, data.z);
	}

	void GLGraphics::setFloat3(Shader* shader, const char* name, float* data)
	{
		glUniform3fv(glGetUniformLocation(shader->getProgramID(), name), 1, data);
	}

	void GLGraphics::setFloat3(Shader* shader, const int& shaderID, const float* data)
	{
		TINY_GL_CHECK(glUniform3fv(shaderID, 1, data));
	}

	void GLGraphics::setFloat4(Shader* shader, UniformID& uniform, float* data)
	{
		int shader_id;
		if (!shader->checkTinyUniform(uniform, shader_id))
		{
			return;
		}
		glUniform4fv(shader_id, 1, data);
	}

	void GLGraphics::setFloat4(Shader* shader, const char* name, float* data)
	{
		glUniform4fv(glGetUniformLocation(shader->getProgramID(), name), 1, data);
	}

	void GLGraphics::setFloat4(Shader* shader, const int& shaderID, const float* data)
	{
		glUniform4fv(shaderID, 1, data);
	}

	void GLGraphics::setInt1(Shader* shader, UniformID& uniform, const int& data)
	{
		int shader_id;
		if (!shader->checkTinyUniform(uniform, shader_id))
		{
			return;
		}
		glUniform1i(shader_id, data);
	}

	void GLGraphics::setInt1(Shader* shader, UniformID& uniform, int* data)
	{
		int shader_id;
		if (!shader->checkTinyUniform(uniform, shader_id))
		{
			return;
		}
		glUniform1iv(shader_id, 1, data);
	}

	void GLGraphics::setInt1(Shader* shader, const char* name, int* data)
	{
		glUniform1iv(glGetUniformLocation(shader->getProgramID(), name), 1, data);
	}

	void GLGraphics::setInt1(Shader* shader, const int& shaderID, int* data)
	{
		glUniform1iv(shaderID, 1, data);
	}

	void GLGraphics::setInt1(Shader* shader, const char* name, const int& data)
	{
		glUniform1i(glGetUniformLocation(shader->getProgramID(), name), data);
	}

	void GLGraphics::setInt1(Shader* shader, const int& shaderID, const int& data)
	{
		glUniform1i(shaderID, data);
	}

	void GLGraphics::setInt2(Shader* shader, UniformID& uniform, int* data)
	{
		int shader_id;
		if (!shader->checkTinyUniform(uniform, shader_id))
		{
			return;
		}
		glUniform2iv(shader_id, 1, data);
	}

	void GLGraphics::setInt2(Shader* shader, const char* name, int* data)
	{
		glUniform2iv(glGetUniformLocation(shader->getProgramID(), name), 1, data);
	}

	void GLGraphics::setInt2(Shader* shader, const int& shaderID, int* data)
	{
		glUniform2iv(shaderID, 1, data);
	}

	void GLGraphics::setInt3(Shader* shader, UniformID& uniform, int* data)
	{
		int shader_id;
		if (!shader->checkTinyUniform(uniform, shader_id))
		{
			return;
		}
		glUniform3iv(shader_id, 1, data);
	}

	void GLGraphics::setInt3(Shader* shader, const char* name, int* data)
	{
		glUniform3iv(glGetUniformLocation(shader->getProgramID(), name), 1, data);
	}

	void GLGraphics::setInt3(Shader* shader, const int& shaderID, int* data)
	{
		glUniform3iv(shaderID, 1, data);
	}

	void GLGraphics::setInt4(Shader* shader, UniformID& uniform, int* data)
	{
		int shader_id;
		if (!shader->checkTinyUniform(uniform, shader_id))
		{
			return;
		}
		glUniform4iv(shader_id, 1, data);
	}

	void GLGraphics::setInt4(Shader* shader, const char* name, int* data)
	{
		glUniform4iv(glGetUniformLocation(shader->getProgramID(), name), 1, data);
	}

	void GLGraphics::setInt4(Shader* shader, const int& shaderID, int* data)
	{
		glUniform4iv(shaderID, 1, data);
	}

	void GLGraphics::setMat3(Shader* shader, UniformID& uniform, float* data)
	{
		int shader_id;
		if (!shader->checkTinyUniform(uniform, shader_id))
		{
			return;
		}
		glUniformMatrix3fv(shader_id, 1, GL_FALSE, data);
	}

	void GLGraphics::setMat3(Shader* shader, UniformID& uniform, const float3x3& mat3)
	{
		int shader_id;
		if (!shader->checkTinyUniform(uniform, shader_id))
		{
			return;
		}
		glUniformMatrix3fv(shader_id, 1, GL_FALSE, glm::value_ptr(mat3));
	}

	void GLGraphics::setMat3(Shader* shader, const char* name, float* data)
	{
		glUniformMatrix3fv(glGetUniformLocation(shader->getProgramID(), name), 1, GL_FALSE, data);
	}

	void GLGraphics::setMat3(Shader* shader, const int& shaderID, const float* data)
	{
		glUniformMatrix3fv(shaderID, 1, GL_FALSE, data);
	}

	void GLGraphics::setMat3(Shader* shader, const int& shaderID, const float3x3& mat3)
	{
		glUniformMatrix3fv(shaderID, 1, GL_FALSE, glm::value_ptr(mat3));
	}

	void GLGraphics::setMat4(Shader* shader, UniformID& uniform, const float* data)
	{
		int shader_id;
		if (!shader->checkTinyUniform(uniform, shader_id))
		{
			return;
		}
		glUniformMatrix4fv(shader_id, 1, GL_FALSE, data);
	}

	void GLGraphics::setMat4(Shader* shader, UniformID& uniform, const float4x4& mat4)
	{
		int shader_id;
		if (!shader->checkTinyUniform(uniform, shader_id))
		{
			return;
		}
		glUniformMatrix4fv(shader_id, 1, GL_FALSE, glm::value_ptr(mat4));
	}

	void GLGraphics::setMat4(Shader* shader, UniformID& uniform, float4x4 data[], int count)
	{
		int shader_id;
		if (!shader->checkTinyUniform(uniform, shader_id))
		{
			return;
		}
		glUniformMatrix4fv(shader_id, count, GL_FALSE, glm::value_ptr(data[0]));
	}

	void GLGraphics::setMat4(Shader* shader, const char* name, const float* data)
	{
		glUniformMatrix4fv(glGetUniformLocation(shader->getProgramID(), name), 1, GL_FALSE, data);
	}

	void GLGraphics::setMat4(Shader* shader, const int& shaderID, const float* data)
	{
		glUniformMatrix4fv(shaderID, 1, GL_FALSE, data);
	}

	void GLGraphics::setMat4(Shader* shader, const int& shaderID, const float4x4& mat4)
	{
		glUniformMatrix4fv(shaderID, 1, GL_FALSE, glm::value_ptr(mat4));
	}

	void GLGraphics::setMat4(Shader* shader, const int& shaderID, float4x4 data[], int count)
	{
		glUniformMatrix4fv(shaderID, count, GL_FALSE, glm::value_ptr(data[0]));
	}

	//---------------------------------------------------
	//
	//	Render CMD
	//
	RenderCommand* GLGraphics::createDrawVertexCMD(Vertex* vertex)
	{
		return new GLRenderCMD_Vertex(vertex);
	}

	RenderCommand* GLGraphics::createDrawShadowCMD(Vertex* vertex, Transform* transform)
	{
		return new GLRenderCMD_Shadow(vertex, transform);
	}

	RenderCommand* GLGraphics::createDrawMeshCMD(Vertex* vertex, Transform* transform, Material* material)
	{
		return new GLRenderCMD_Mesh(vertex, transform, material);
	}

	RenderCommand* GLGraphics::createDrawSkyboxCMD(Vertex* vertex, TextureCube* cube, float lod, bool isHdr, float exposure)
	{
		return new GLRenderCMD_Skybox(vertex, cube, lod, isHdr, exposure);
	}

	RenderCommand* GLGraphics::createDrawHDRToCubeCMD(Vertex* vertex, Texture2D* hdr, TextureCube* cube)
	{
		return new GLRenderCMD_HDRToCube(vertex, hdr, cube);
	}

	RenderCommand* GLGraphics::createDrawEnvMakeIrradiance(Vertex* vertex, TextureCube* cube, TextureCube* irradiance)
	{
		return new GLRenderCMD_EnvMakeIrradiance(vertex, cube, irradiance);
	}

	RenderCommand* GLGraphics::createDrawEnvMakePrefilter(Vertex* vertex, TextureCube* cube, TextureCube* prefitler, uint32 mipMaxLevel, uint32 mipWidth, uint32 mipHeight, float resolution)
	{
		return new GLRenderCMD_EnvMakePrefilter(vertex, cube, prefitler, mipMaxLevel, mipWidth, mipHeight, resolution);
	}

}
