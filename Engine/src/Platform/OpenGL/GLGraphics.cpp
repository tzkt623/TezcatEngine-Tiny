/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Platform/OpenGL/GLGraphics.h"

#include "Platform/OpenGL/GLHead.h"
#include "Platform/OpenGL/GLFunction.h"

#include "Core/Head/CppHead.h"

#include "Core/Component/MeshRenderer.h"
#include "Core/Component/Camera.h"

#include "Core/Engine.h"
#include "Core/Debug/Debug.h"

#include "Core/Shader/ShaderPackage.h"
#include "Core/Shader/Shader.h"

#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Texture.h"
#include "Core/Renderer/FrameBuffer.h"
#include "Core/Renderer/Vertex.h"
#include "Core/Renderer/VertexBuffer.h"
#include "Core/Renderer/RenderConfig.h"


#include "Core/EngineIniter.h"

#include "Core/Manager/LightingManager.h"

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
			TINY_LOG_ERROR("Failed to create GLFW window");
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(mWindow);
		glfwSwapInterval(engine->getResourceLoader()->isEnabelVsync() ? 1 : 0);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			TINY_LOG_ERROR("Failed to initialize GLAD");
			TINY_THROW_LOGIC("Failed to initialize GLAD");
		}

		GLint max;
		glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &max);
		//std::cout << "GL_MAX_UNIFORM_LOCATIONS: " << max << std::endl;
		TINY_LOG_ENGINE(StringTool::stringFormat("GL_MAX_UNIFORM_LOCATIONS: %d", max));

		auto get_default_color_buffer = [&](GLint id)
			{
				switch (id)
				{
				case GL_NONE:
					//std::cout << "Default ColorBuffer: GL_NONE" << std::endl;
					TINY_LOG_ENGINE("Default ColorBuffer: GL_NONE");
					break;
				case GL_FRONT_LEFT:
					//std::cout << "Default ColorBuffer: GL_FRONT_LEFT" << std::endl;
					TINY_LOG_ENGINE("Default ColorBuffer: GL_FRONT_LEFT");
					break;
				case GL_FRONT_RIGHT:
					//std::cout << "Default ColorBuffer: GL_FRONT_RIGHT" << std::endl;
					TINY_LOG_ENGINE("Default ColorBuffer: GL_FRONT_RIGHT");
					break;
				case GL_BACK_LEFT:
					//std::cout << "Default ColorBuffer: GL_BACK_LEFT" << std::endl;
					TINY_LOG_ENGINE("Default ColorBuffer: GL_BACK_LEFT");
					break;
				case GL_BACK_RIGHT:
					//std::cout << "Default ColorBuffer: GL_BACK_RIGHT" << std::endl;
					TINY_LOG_ENGINE("Default ColorBuffer: GL_BACK_RIGHT");
					break;
				case GL_FRONT:
					//std::cout << "Default ColorBuffer: GL_FRONT" << std::endl;
					TINY_LOG_ENGINE("Default ColorBuffer: GL_FRONT");
					break;
				case GL_BACK:
					//std::cout << "Default ColorBuffer: GL_BACK" << std::endl;
					TINY_LOG_ENGINE("Default ColorBuffer: GL_BACK");
					break;
				case GL_LEFT:
					//std::cout << "Default ColorBuffer: GL_LEFT" << std::endl;
					TINY_LOG_ENGINE("Default ColorBuffer: GL_LEFT");
					break;
				case GL_RIGHT:
					//std::cout << "Default ColorBuffer: GL_RIGHT" << std::endl;
					TINY_LOG_ENGINE("Default ColorBuffer: GL_RIGHT");
					break;
				case GL_FRONT_AND_BACK:
					//std::cout << "Default ColorBuffer: GL_FRONT_AND_BACK" << std::endl;
					TINY_LOG_ENGINE("Default ColorBuffer: GL_FRONT_AND_BACK");
					break;
				default:
					break;
				}
			};

		glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &max);
		//std::cout << "GL_MAX_UNIFORM_LOCATIONS: " << max << std::endl;
		TINY_LOG_ENGINE(StringTool::stringFormat("GL_MAX_UNIFORM_LOCATIONS: %d", max));

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
			TexInternalFormatWrapper(TextureInternalFormat::None,			GL_NONE),
			TexInternalFormatWrapper(TextureInternalFormat::Depth,			GL_DEPTH_COMPONENT),
			TexInternalFormatWrapper(TextureInternalFormat::Depth_Stencil,	GL_DEPTH_STENCIL),
			TexInternalFormatWrapper(TextureInternalFormat::R,				GL_RED),
			TexInternalFormatWrapper(TextureInternalFormat::RG,				GL_RG),
			TexInternalFormatWrapper(TextureInternalFormat::RGB,			GL_RGB),
			TexInternalFormatWrapper(TextureInternalFormat::RGBA,			GL_RGBA),
			TexInternalFormatWrapper(TextureInternalFormat::R8,				GL_R8),
			TexInternalFormatWrapper(TextureInternalFormat::RG8,			GL_RG8),
			TexInternalFormatWrapper(TextureInternalFormat::RGB8,			GL_RGB8),
			TexInternalFormatWrapper(TextureInternalFormat::RGBA8,			GL_RGBA8),
			TexInternalFormatWrapper(TextureInternalFormat::R16,			GL_R16),
			TexInternalFormatWrapper(TextureInternalFormat::RG16,			GL_RG16),
			TexInternalFormatWrapper(TextureInternalFormat::RGB16,			GL_RGB16),
			TexInternalFormatWrapper(TextureInternalFormat::RGBA16,			GL_RGBA16),
			TexInternalFormatWrapper(TextureInternalFormat::R16F,			GL_R16F),
			TexInternalFormatWrapper(TextureInternalFormat::RG16F,			GL_RG16F),
			TexInternalFormatWrapper(TextureInternalFormat::RGB16F,			GL_RGB16F),
			TexInternalFormatWrapper(TextureInternalFormat::RGBA16F,		GL_RGBA16F),
			TexInternalFormatWrapper(TextureInternalFormat::R32F,			GL_R32F),
			TexInternalFormatWrapper(TextureInternalFormat::RG32F,			GL_RG32F),
			TexInternalFormatWrapper(TextureInternalFormat::RGB32F,			GL_RGB32F),
			TexInternalFormatWrapper(TextureInternalFormat::RGBA32F,		GL_RGBA32F),
			TexInternalFormatWrapper(TextureInternalFormat::R32UI,			GL_R32UI),
		};

		ContextMap::TextureFormatArray =
		{
			TexFormatWrapper(TextureFormat::None,			GL_NONE),
			TexFormatWrapper(TextureFormat::Depth,			GL_DEPTH_COMPONENT),
			TexFormatWrapper(TextureFormat::Stencil,		GL_STENCIL_INDEX),
			TexFormatWrapper(TextureFormat::Depth_Stencil,	GL_DEPTH_STENCIL),
			TexFormatWrapper(TextureFormat::R,				GL_RED),
			TexFormatWrapper(TextureFormat::RG,				GL_RG),
			TexFormatWrapper(TextureFormat::RGB,			GL_RGB),
			TexFormatWrapper(TextureFormat::RGBA,			GL_RGBA),
			TexFormatWrapper(TextureFormat::BGR,			GL_BGR),
			TexFormatWrapper(TextureFormat::BGRA,			GL_BGRA),
			TexFormatWrapper(TextureFormat::RI,				GL_RED_INTEGER),
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
		TINY_GL_CHECK(glUseProgram(shader->getProgramID()));
	}

	void GLGraphics::bind(FrameBuffer* frameBuffer)
	{
		TINY_GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->getFrameBufferID()));
	}

	void GLGraphics::bind(UniformBuffer* uniformBuffer)
	{
		TINY_GL_CHECK(glBindBufferBase(GL_UNIFORM_BUFFER
			, uniformBuffer->getLayout()->mBindingIndex
			, uniformBuffer->getBufferID()));
	}

	void GLGraphics::bind(Texture2D* texture)
	{
		TINY_GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture->getTextureID()));
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
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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

		switch (vertex->getIndexCount())
		{
		case 0u:
			TINY_GL_CHECK(glDrawArrays(vertex->getDrawMode().platform, 0, vertex->getVertexCount()));
			break;
		default:
			TINY_GL_CHECK(glDrawElements(vertex->getDrawMode().platform, vertex->getIndexCount(), GL_UNSIGNED_INT, nullptr));
			break;
		}

		//if (vertex->getIndexCount() > 0)
		//{
		//	TINY_GL_CHECK(glDrawElements(vertex->getDrawMode().platform, vertex->getIndexCount(), GL_UNSIGNED_INT, nullptr));
		//}
		//else
		//{
		//	TINY_GL_CHECK(glDrawArrays(vertex->getDrawMode().platform, 0, vertex->getVertexCount()));
		//}

		TINY_PROFILER_DRAWCALL(1);
	}

	void GLGraphics::drawLine(Vertex* vertex, const uint32& needCount)
	{
		glBindVertexArray(vertex->getVertexID());
		glDrawArrays(GL_LINE, 0, needCount);
	}

	void GLGraphics::drawLine(const glm::vec3& begin, const glm::vec3& end, const glm::vec3& color /*= glm::vec3(0.0f, 1.0f, 0.0f)*/)
	{

	}

#pragma region Update
	void GLGraphics::cmdUpdateTexture2D(Texture2D* tex2d)
	{
		//this->createCMD<GLBuildCMD_UpdateTextrue2D>(tex2d);
	}
#pragma endregion

#pragma region Create
	//---------------------------------------------------
	//
	//	Render Function
	//
	void GLGraphics::createVertex(Vertex* vertex)
	{
		GLFunction::FuncBuffer.build(vertex);
	}

	void GLGraphics::createBuffer(VertexBuffer* buffer)
	{
		GLFunction::FuncBuffer.build(buffer);
	}

	void GLGraphics::createBuffer(IndexBuffer* buffer)
	{
		GLFunction::FuncBuffer.build(buffer);
	}

	void GLGraphics::createBuffer(FrameBuffer* buffer)
	{
		GLFunction::FuncFrameBuffer.build(buffer);
	}

	void GLGraphics::createUniformBuffer(UniformBuffer* uniformBuffer, int32_t bindingIndex)
	{
		if (uniformBuffer->getBufferID() > 0)
		{
			TINY_GL_CHECK(glDeleteBuffers(1, &uniformBuffer->getBufferID()));
		}

		GLuint ubo;
		TINY_GL_CHECK(glGenBuffers(1, &ubo));
		TINY_GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, ubo));
		TINY_GL_CHECK(glBufferData(GL_UNIFORM_BUFFER
			, uniformBuffer->getDataSize()
			, uniformBuffer->getData()
			, GL_STATIC_DRAW));

		if (bindingIndex > -1)
		{
			TINY_GL_CHECK(glBindBufferBase(GL_UNIFORM_BUFFER, bindingIndex, ubo));
		}

		uniformBuffer->apply(ubo);
		TINY_GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, 0));
	}

	void GLGraphics::createShader(Shader* shader)
	{
		GLFunction::FuncShader.build(shader);
	}

	void GLGraphics::createTexture(Texture2D* tex2D)
	{
		GLFunction::FuncTexture.build(tex2D);
	}

	void GLGraphics::createTexture(TextureCube* texCube)
	{
		GLFunction::FuncTexture.build(texCube);
	}

	void GLGraphics::createTexture(TextureRender2D* render2D)
	{
		GLFunction::FuncTexture.build(render2D);
	}
#pragma endregion

#pragma region Delete
	void GLGraphics::deleteShader(const uint32_t& id)
	{
		TINY_GL_CHECK(glDeleteProgram(id));
	}

	void GLGraphics::deleteVertex(const uint32_t& id)
	{
		TINY_GL_CHECK(glDeleteVertexArrays(1, &id));
	}

	void GLGraphics::deleteTexture2D(const uint32_t& id)
	{
		TINY_GL_CHECK(glDeleteTextures(1, &id));
	}

	void GLGraphics::deleteTextureCube(const uint32_t& id)
	{
		TINY_GL_CHECK(glDeleteTextures(1, &id));
	}

	void GLGraphics::deleteRender2D(const uint32_t& id)
	{
		TINY_GL_CHECK(glDeleteRenderbuffers(1, &id));
	}

	void GLGraphics::deleteFrameBuffer(const uint32_t& id)
	{
		TINY_GL_CHECK(glDeleteFramebuffers(1, &id));
	}

	void GLGraphics::deleteVertexBuffer(const uint32_t& id)
	{
		TINY_GL_CHECK(glDeleteBuffers(1, &id));
	}

	void GLGraphics::deleteIndexBuffer(const uint32_t& id)
	{
		TINY_GL_CHECK(glDeleteBuffers(1, &id));
	}

	void GLGraphics::deleteUniformBuffer(const uint32_t& id)
	{
		TINY_GL_CHECK(glDeleteBuffers(1, &id));
	}
#pragma endregion



#pragma region Draw
	void GLGraphics::makeHDR2Cube(Shader* shader
		, BaseRenderObserver* observer
		, Vertex* vertex
		, Texture2D* texHDR
		, TextureCube* skybox)
	{
		shader->resetLocalState();
		auto uniform_config = shader->getUniformValueConfig("myTexHDR2D");
		this->setTexture2D(shader, uniform_config->valueID, texHDR);

		UniformBuffer* uniform_buffer = observer->getUniformBuffer();
		Texture2D** array = LightingManager::getCubeMapTextureArray();

		//auto id1 = glGetUniformBlockIndex(shader->getProgramID(), "SkyboxUBO");
		//GLint index1;
		//glGetActiveUniformBlockiv(shader->getProgramID(), id1, GL_UNIFORM_BLOCK_BINDING, &index1);
		//auto id2 = glGetUniformBlockIndex(shader->getProgramID(), "CameraUBO");
		//GLint index2;
		//glGetActiveUniformBlockiv(shader->getProgramID(), id2, GL_UNIFORM_BLOCK_BINDING, &index2);

		//TINY_GL_CHECK(glBindBufferBase(GL_UNIFORM_BUFFER
		//	, uniform_buffer->getLayout()->mBindingIndex
		//	, uniform_buffer->getBufferID()));

		for (uint32 i = 0; i < 6; i++)
		{
			uniform_buffer->updateWithConfig<UniformBufferBinding::SkyBox::ViewIndex>(&i);
			this->setUniformBuffer(uniform_buffer);

			glFramebufferTexture2D(GL_FRAMEBUFFER
				 , GL_COLOR_ATTACHMENT0
				 , GL_TEXTURE_2D
				 , array[i]->getTextureID()
				 , 0);
			
			this->clear(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
			this->draw(vertex);

			glFramebufferTexture2D(GL_FRAMEBUFFER
				, GL_COLOR_ATTACHMENT0
				, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i
				, skybox->getTextureID()
				, 0);

			this->clear(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
			this->draw(vertex);


			//因为这里的原因
			//texture的ID与槽的自动绑定系统要重新设计
			//因为如果在中途改变一次tex
			//那么以前绑定的就会失效
			//大概率因为默认会绑定到0号贴图槽上
			{
				//glFinish();
				//auto tex = array[i];
				//glBindTexture(GL_TEXTURE_2D, tex->getTextureID());
				//glCopyTexSubImage2D(GL_TEXTURE_2D, 0,
				//	0, 0,
				//	0, 0,
				//	tex->getWidth(), tex->getHeight());
				//glBindTexture(GL_TEXTURE_2D, 0);
			}
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getTextureID());
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 5);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void GLGraphics::makeEnvIrradiance(Shader* shader
		, BaseRenderObserver* observer
		, Vertex* vertex
		, TextureCube* skybox
		, TextureCube* irradiance)
	{
		shader->resetLocalState();
		this->setTextureCube(shader, ShaderParam::TexSkybox, skybox);
		UniformBuffer* uniform_buffer = observer->getUniformBuffer();
		//TINY_GL_CHECK(glBindBufferBase(GL_UNIFORM_BUFFER
		//	, uniform_buffer->getLayout()->mBindingIndex
		//	, uniform_buffer->getBufferID()));

		for (uint32 i = 0; i < 6; i++)
		{
			uniform_buffer->updateWithConfig<UniformBufferBinding::SkyBox::ViewIndex>(&i);
			this->setUniformBuffer(uniform_buffer);

			glFramebufferTexture2D(GL_FRAMEBUFFER
								 , GL_COLOR_ATTACHMENT0
								 , GL_TEXTURE_CUBE_MAP_POSITIVE_X + i
								 , irradiance->getTextureID()
								 , 0);


			this->clear(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
			this->draw(vertex);
		}
	}

	void GLGraphics::makeEnvPrefilter(Shader* shader
		, BaseRenderObserver* observer
		, Vertex* vertex
		, TextureCube* skybox
		, TextureCube* prefitler
		, uint32 mipMaxLevel
		, uint32 mipWidth
		, uint32 mipHeight
		, int32 resolution)
	{
		shader->resetLocalState();
		this->setTextureCube(shader, ShaderParam::TexSkybox, skybox);
		UniformBuffer* uniform_buffer = observer->getUniformBuffer();

		//TINY_GL_CHECK(glBindBufferBase(GL_UNIFORM_BUFFER
		//	, uniform_buffer->getLayout()->mBindingIndex
		//	, uniform_buffer->getBufferID()))

		for (uint32 mip = 0; mip < mipMaxLevel; ++mip)
		{
			uint32 mip_width = static_cast<uint32>(mipWidth * std::pow(0.5, mip));
			uint32 mip_height = static_cast<uint32>(mipHeight * std::pow(0.5, mip));
			this->setViewport(ViewportInfo(0, 0, mip_width, mip_height));

			float roughness = (float)mip / (float)(mipMaxLevel - 1);

			uniform_buffer->updateWithConfig<UniformBufferBinding::SkyBox::Roughness>(&roughness);
			uniform_buffer->updateWithConfig<UniformBufferBinding::SkyBox::Resolution>(&resolution);

			for (uint32 i = 0; i < 6; ++i)
			{
				uniform_buffer->updateWithConfig<UniformBufferBinding::SkyBox::ViewIndex>(&i);
				this->setUniformBuffer(uniform_buffer);

				glFramebufferTexture2D(GL_FRAMEBUFFER
									 , GL_COLOR_ATTACHMENT0
									 , GL_TEXTURE_CUBE_MAP_POSITIVE_X + i
									 , prefitler->getTextureID()
									 , mip);


				this->clear(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
				this->draw(vertex);
			}
		}
	}
#pragma endregion


	void GLGraphics::setGlobalTexture2D(Shader* shader, UniformID& uniform, Texture2D* data)
	{
		int value_id;
		if (!shader->checkTinyUniform(uniform, value_id))
		{
			return;
		}

		TINY_GL_CHECK(glActiveTexture(GL_TEXTURE0 + shader->getTextureIndex()));
		TINY_GL_CHECK(glBindTexture(GL_TEXTURE_2D, data->getTextureID()));
		TINY_GL_CHECK(glUniform1i(value_id, shader->getTextureIndex()));
		shader->addGlobalTextureIndex();
	}

	void GLGraphics::setGlobalTexture2D(Shader* shader, const int& valueID, Texture2D* data)
	{
		TINY_GL_CHECK(glActiveTexture(GL_TEXTURE0 + shader->getTextureIndex()));
		TINY_GL_CHECK(glBindTexture(GL_TEXTURE_2D, data->getTextureID()));
		TINY_GL_CHECK(glUniform1i(valueID, shader->getTextureIndex()));
		shader->addGlobalTextureIndex();
	}

	void GLGraphics::setTexture2D(Shader* shader, UniformID& uniform, Texture2D* data)
	{
		int value_id;
		if (!shader->checkTinyUniform(uniform, value_id))
		{
			return;
		}

		TINY_GL_CHECK(glActiveTexture(GL_TEXTURE0 + shader->getTextureIndex()));
		TINY_GL_CHECK(glBindTexture(GL_TEXTURE_2D, data->getTextureID()));
		TINY_GL_CHECK(glUniform1i(value_id, shader->getTextureIndex()));
		shader->addLocalTextureIndex();
	}

	void GLGraphics::setTexture2D(Shader* shader, const int32_t& valueID, Texture2D* data)
	{
		TINY_GL_CHECK(glActiveTexture(GL_TEXTURE0 + shader->getTextureIndex()));
		TINY_GL_CHECK(glBindTexture(GL_TEXTURE_2D, data->getTextureID()));
		TINY_GL_CHECK(glUniform1i(valueID, shader->getTextureIndex()));
		shader->addLocalTextureIndex();
	}

	void GLGraphics::setGlobalTextureCube(Shader* shader, UniformID& uniform, TextureCube* data)
	{
		int32_t value_id;
		if (!shader->checkTinyUniform(uniform, value_id))
		{
			return;
		}

		TINY_GL_CHECK(glActiveTexture(GL_TEXTURE0 + shader->getTextureIndex()));
		TINY_GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, data->getTextureID()));
		TINY_GL_CHECK(glUniform1i(value_id, shader->getTextureIndex()));
		shader->addGlobalTextureIndex();
	}

	void GLGraphics::setGlobalTextureCube(Shader* shader, const int32_t& valueID, TextureCube* data)
	{
		TINY_GL_CHECK(glActiveTexture(GL_TEXTURE0 + shader->getTextureIndex()));
		TINY_GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, data->getTextureID()));
		TINY_GL_CHECK(glUniform1i(valueID, shader->getTextureIndex()));
		shader->addGlobalTextureIndex();
	}

	void GLGraphics::setTextureCube(Shader* shader, UniformID& uniform, TextureCube* data)
	{
		int32_t value_id;
		if (!shader->checkTinyUniform(uniform, value_id))
		{
			return;
		}

		TINY_GL_CHECK(glActiveTexture(GL_TEXTURE0 + shader->getTextureIndex()));
		TINY_GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, data->getTextureID()));
		TINY_GL_CHECK(glUniform1i(value_id, shader->getTextureIndex()));
		shader->addLocalTextureIndex();
	}

	void GLGraphics::setTextureCube(Shader* shader, const int32_t& valueID, TextureCube* data)
	{
		TINY_GL_CHECK(glActiveTexture(GL_TEXTURE0 + shader->getTextureIndex()));
		TINY_GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, data->getTextureID()));
		TINY_GL_CHECK(glUniform1i(valueID, shader->getTextureIndex()));
		shader->addLocalTextureIndex();
	}

	void GLGraphics::setBool(Shader* shader, const char* name, const bool& data)
	{
		TINY_GL_CHECK_RETURN(glGetUniformLocation(shader->getProgramID(), name), id);
		TINY_GL_CHECK(glUniform1i(id, data));
	}

	void GLGraphics::setFloat1(Shader* shader, UniformID& uniform, float* data)
	{
		int32_t value_id;
		if (!shader->checkTinyUniform(uniform, value_id))
		{
			return;
		}
		TINY_GL_CHECK(glUniform1fv(value_id, 1, data));
	}

	void GLGraphics::setFloat1(Shader* shader, const char* name, float* data)
	{
		TINY_GL_CHECK_RETURN(glGetUniformLocation(shader->getProgramID(), name), id);
		TINY_GL_CHECK(glUniform1fv(id, 1, data));
	}

	void GLGraphics::setFloat1(Shader* shader, const int32_t& valueID, const float* data)
	{
		TINY_GL_CHECK(glUniform1fv(valueID, 1, data));
	}

	void GLGraphics::setFloat1(Shader* shader, const char* name, const float& data)
	{
		TINY_GL_CHECK_RETURN(glGetUniformLocation(shader->getProgramID(), name), id);
		TINY_GL_CHECK(glUniform1f(id, data));
	}

	void GLGraphics::setFloat1(Shader* shader, const int32_t& valueID, const float& data)
	{
		TINY_GL_CHECK(glUniform1f(valueID, data));
	}

	void GLGraphics::setFloat2(Shader* shader, UniformID& uniform, float* data)
	{
		int32_t value_id;
		if (!shader->checkTinyUniform(uniform, value_id))
		{
			return;
		}
		TINY_GL_CHECK(glUniform2fv(value_id, 1, data));
	}

	void GLGraphics::setFloat2(Shader* shader, UniformID& uniform, const glm::vec2& data)
	{
		int32_t value_id;
		if (!shader->checkTinyUniform(uniform, value_id))
		{
			return;
		}
		TINY_GL_CHECK(glUniform2f(value_id, data.x, data.y));
	}

	void GLGraphics::setFloat2(Shader* shader, const char* name, float* data)
	{
		TINY_GL_CHECK_RETURN(glGetUniformLocation(shader->getProgramID(), name), id);
		TINY_GL_CHECK(glUniform2fv(id, 1, data));
	}

	void GLGraphics::setFloat2(Shader* shader, const int32_t& valueID, const float* data)
	{
		TINY_GL_CHECK(glUniform2fv(valueID, 1, data));
	}

	void GLGraphics::setFloat2(Shader* shader, const int32_t& valueID, const float2& data)
	{
		TINY_GL_CHECK(glUniform2fv(valueID, 1, &data.x));
	}

	void GLGraphics::setFloat3(Shader* shader, UniformID& uniform, float* data)
	{
		int32_t value_id;
		if (!shader->checkTinyUniform(uniform, value_id))
		{
			return;
		}
		TINY_GL_CHECK(glUniform3fv(value_id, 1, data));
	}

	void GLGraphics::setFloat3(Shader* shader, UniformID& uniform, const glm::vec3& data)
	{
		int32_t value_id;
		if (!shader->checkTinyUniform(uniform, value_id))
		{
			return;
		}
		TINY_GL_CHECK(glUniform3f(value_id, data.x, data.y, data.z));
	}

	void GLGraphics::setFloat3(Shader* shader, const char* name, float* data)
	{
		TINY_GL_CHECK(glUniform3fv(glGetUniformLocation(shader->getProgramID(), name), 1, data));
	}

	void GLGraphics::setFloat3(Shader* shader, const int32_t& valueID, const float* data)
	{
		TINY_GL_CHECK(glUniform3fv(valueID, 1, data));
	}

	void GLGraphics::setFloat3(Shader* shader, const int32_t& valueID, const float3& data)
	{
		TINY_GL_CHECK(glUniform3fv(valueID, 1, &data.x));
	}

	void GLGraphics::setFloat4(Shader* shader, UniformID& uniform, float* data)
	{
		int32_t value_id;
		if (!shader->checkTinyUniform(uniform, value_id))
		{
			return;
		}
		TINY_GL_CHECK(glUniform4fv(value_id, 1, data));
	}

	void GLGraphics::setFloat4(Shader* shader, const char* name, float* data)
	{
		TINY_GL_CHECK(glUniform4fv(glGetUniformLocation(shader->getProgramID(), name), 1, data));
	}

	void GLGraphics::setFloat4(Shader* shader, const int32_t& valueID, const float* data)
	{
		TINY_GL_CHECK(glUniform4fv(valueID, 1, data));
	}

	void GLGraphics::setFloat4(Shader* shader, const int32_t& valueID, const float4& data)
	{
		TINY_GL_CHECK(glUniform4fv(valueID, 1, &data.x));
	}

	void GLGraphics::setUInt1(Shader* shader, const char* name, const uint32_t& data)
	{
		auto id = glGetUniformLocation(shader->getProgramID(), name);
		TINY_GL_CHECK(glUniform1ui(id, data));

		//glReadBuffer(GL_COLOR_ATTACHMENT0);
		//uint32_t index = 0;
		//glReadPixels(0, 0, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &index);
		//glReadBuffer(GL_NONE);
		//std::cerr << index << "\n";
	}

	void GLGraphics::setInt1(Shader* shader, UniformID& uniform, const int32_t& data)
	{
		int32_t value_id;
		if (!shader->checkTinyUniform(uniform, value_id))
		{
			return;
		}
		TINY_GL_CHECK(glUniform1i(value_id, data));
	}

	void GLGraphics::setInt1(Shader* shader, UniformID& uniform, int32_t* data)
	{
		int32_t value_id;
		if (!shader->checkTinyUniform(uniform, value_id))
		{
			return;
		}
		TINY_GL_CHECK(glUniform1iv(value_id, 1, data));
	}

	void GLGraphics::setInt1(Shader* shader, const char* name, int32_t* data)
	{
		TINY_GL_CHECK(glUniform1iv(glGetUniformLocation(shader->getProgramID(), name), 1, data));
	}

	void GLGraphics::setInt1(Shader* shader, const int32_t& valueID, int32_t* data)
	{
		TINY_GL_CHECK(glUniform1iv(valueID, 1, data));
	}

	void GLGraphics::setInt1(Shader* shader, const char* name, const int32_t& data)
	{
		TINY_GL_CHECK(glUniform1i(glGetUniformLocation(shader->getProgramID(), name), data));
	}

	void GLGraphics::setInt1(Shader* shader, const int32_t& valueID, const int32_t& data)
	{
		TINY_GL_CHECK(glUniform1i(valueID, data));
	}

	void GLGraphics::setInt2(Shader* shader, UniformID& uniform, int32_t* data)
	{
		int32_t value_id;
		if (!shader->checkTinyUniform(uniform, value_id))
		{
			return;
		}
		TINY_GL_CHECK(glUniform2iv(value_id, 1, data));
	}

	void GLGraphics::setInt2(Shader* shader, const char* name, int32_t* data)
	{
		TINY_GL_CHECK(glUniform2iv(glGetUniformLocation(shader->getProgramID(), name), 1, data));
	}

	void GLGraphics::setInt2(Shader* shader, const int32_t& valueID, int32_t* data)
	{
		TINY_GL_CHECK(glUniform2iv(valueID, 1, data));
	}

	void GLGraphics::setInt3(Shader* shader, UniformID& uniform, int32_t* data)
	{
		int32_t value_id;
		if (!shader->checkTinyUniform(uniform, value_id))
		{
			return;
		}
		TINY_GL_CHECK(glUniform3iv(value_id, 1, data));
	}

	void GLGraphics::setInt3(Shader* shader, const char* name, int32_t* data)
	{
		TINY_GL_CHECK(glUniform3iv(glGetUniformLocation(shader->getProgramID(), name), 1, data));
	}

	void GLGraphics::setInt3(Shader* shader, const int32_t& valueID, int32_t* data)
	{
		TINY_GL_CHECK(glUniform3iv(valueID, 1, data));
	}

	void GLGraphics::setInt4(Shader* shader, UniformID& uniform, int32_t* data)
	{
		int32_t value_id;
		if (!shader->checkTinyUniform(uniform, value_id))
		{
			return;
		}
		TINY_GL_CHECK(glUniform4iv(value_id, 1, data));
	}

	void GLGraphics::setInt4(Shader* shader, const char* name, int32_t* data)
	{
		TINY_GL_CHECK(glUniform4iv(glGetUniformLocation(shader->getProgramID(), name), 1, data));
	}

	void GLGraphics::setInt4(Shader* shader, const int32_t& valueID, int32_t* data)
	{
		TINY_GL_CHECK(glUniform4iv(valueID, 1, data));
	}

	void GLGraphics::setMat3(Shader* shader, UniformID& uniform, float* data)
	{
		int32_t value_id;
		if (!shader->checkTinyUniform(uniform, value_id))
		{
			return;
		}
		TINY_GL_CHECK(glUniformMatrix3fv(value_id, 1, GL_FALSE, data));
	}

	void GLGraphics::setMat3(Shader* shader, UniformID& uniform, const float3x3& mat3)
	{
		int32_t value_id;
		if (!shader->checkTinyUniform(uniform, value_id))
		{
			return;
		}
		TINY_GL_CHECK(glUniformMatrix3fv(value_id, 1, GL_FALSE, glm::value_ptr(mat3)));
	}

	void GLGraphics::setMat3(Shader* shader, const char* name, float* data)
	{
		TINY_GL_CHECK(glUniformMatrix3fv(glGetUniformLocation(shader->getProgramID(), name), 1, GL_FALSE, data));
	}

	void GLGraphics::setMat3(Shader* shader, const int32_t& valueID, const float* data)
	{
		TINY_GL_CHECK(glUniformMatrix3fv(valueID, 1, GL_FALSE, data));
	}

	void GLGraphics::setMat3(Shader* shader, const int32_t& valueID, const float3x3& mat3)
	{
		TINY_GL_CHECK(glUniformMatrix3fv(valueID, 1, GL_FALSE, glm::value_ptr(mat3)));
	}

	void GLGraphics::setMat4(Shader* shader, UniformID& uniform, const float* data)
	{
		int32_t value_id;
		if (!shader->checkTinyUniform(uniform, value_id))
		{
			return;
		}
		TINY_GL_CHECK(glUniformMatrix4fv(value_id, 1, GL_FALSE, data));
	}

	void GLGraphics::setMat4(Shader* shader, UniformID& uniform, const float4x4& mat4)
	{
		int32_t value_id;
		if (!shader->checkTinyUniform(uniform, value_id))
		{
			return;
		}
		TINY_GL_CHECK(glUniformMatrix4fv(value_id, 1, GL_FALSE, glm::value_ptr(mat4)));
	}

	void GLGraphics::setMat4(Shader* shader, UniformID& uniform, float4x4 data[], int32_t count)
	{
		int32_t value_id;
		if (!shader->checkTinyUniform(uniform, value_id))
		{
			return;
		}
		TINY_GL_CHECK(glUniformMatrix4fv(value_id, count, GL_FALSE, glm::value_ptr(data[0])));
	}

	void GLGraphics::setMat4(Shader* shader, const char* name, const float* data)
	{
		TINY_GL_CHECK(glUniformMatrix4fv(glGetUniformLocation(shader->getProgramID(), name), 1, GL_FALSE, data));
	}

	void GLGraphics::setMat4(Shader* shader, const int32_t& valueID, const float* data)
	{
		TINY_GL_CHECK(glUniformMatrix4fv(valueID, 1, GL_FALSE, data));
	}

	void GLGraphics::setMat4(Shader* shader, const int32_t& valueID, const float4x4& mat4)
	{
		TINY_GL_CHECK(glUniformMatrix4fv(valueID, 1, GL_FALSE, glm::value_ptr(mat4)));
	}

	void GLGraphics::setMat4(Shader* shader, const int32_t& valueID, float4x4 data[], int32_t count)
	{
		TINY_GL_CHECK(glUniformMatrix4fv(valueID, count, GL_FALSE, glm::value_ptr(data[0])));
	}

	void GLGraphics::setUniformBuffer(UniformBuffer* uniformBuffer)
	{
		TINY_GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer->getBufferID()));
		TINY_GL_CHECK(glBufferSubData(GL_UNIFORM_BUFFER, 0, uniformBuffer->getDataSize(), uniformBuffer->getData()));
		//TINY_GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, 0));
	}

	void GLGraphics::setFrameBufferColorTexture2D(Texture2D* tex, int32_t index)
	{
		TINY_GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER
			, GL_COLOR_ATTACHMENT0 + index
			, GL_TEXTURE_2D
			, tex->getTextureID()
			, 0));
	}

	void GLGraphics::setFrameBufferColorTextureCube(TextureCube* tex, int32_t index, int32_t cubeFace)
	{
		TINY_GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER
			, GL_COLOR_ATTACHMENT0 + index
			, GL_TEXTURE_CUBE_MAP_POSITIVE_X + cubeFace
			, tex->getTextureID()
			, 0));
	}

	void GLGraphics::createMipmapTexCube(TextureCube* texCube, int32_t minLevel, int32_t maxLevel)
	{
		TINY_GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, texCube->getTextureID()));
		TINY_GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, minLevel));
		TINY_GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, maxLevel));
		TINY_GL_CHECK(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
		TINY_GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
	}

	void GLGraphics::readPixel(int32_t x, int32_t y)
	{
		TINY_GL_CHECK(glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr));
	}

	void GLGraphics::readObjectID(int32_t x, int32_t y, int32_t& id)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		TINY_GL_CHECK(glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &id));
		glReadBuffer(GL_NONE);
	}

	void GLGraphics::setClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}
}
