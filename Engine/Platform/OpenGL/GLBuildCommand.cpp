#include "GLBuildCommand.h"
#include "GLHead.h"
#include "GLShaderBuilder.h"

#include "Core/Data/Image.h"
#include "Core/Renderer/Texture.h"
#include "Core/Renderer/FrameBuffer.h"
#include "Core/Renderer/Vertex.h"
#include "Core/Renderer/VertexBuffer.h"
#include "Core/Head/TinyCpp.h"
#include "Core/Tool/Log.h"
#include "Core/Shader/Shader.h"

namespace tezcat::Tiny::GL
{
#pragma region Create
	//---------------------------------------
	//
	//	GLBuildCMD_Vertex
	//
	GLBuildCMD_CreateVertex::GLBuildCMD_CreateVertex(Vertex* vertex)
		: mVertex(vertex)
	{
		mVertex->addRef();
	}

	GLBuildCMD_CreateVertex::~GLBuildCMD_CreateVertex()
	{
		mVertex->subRef();
	}

	void GLBuildCMD_CreateVertex::execute(BaseGraphics* graphcis)
	{
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		for (auto buffer : mVertex->getVertexBuffers())
		{
			if (buffer->getBufferID() == 0)
			{
				GLBuildCMD_CreateVertexBuffer cmd(buffer);
				cmd.execute(graphcis);
			}
			else
			{
				glBindBuffer(GL_ARRAY_BUFFER, buffer->getBufferID());
			}

			auto layout_data = buffer->getLayoutData();
			switch (layout_data.type)
			{
			case  VertexLayoutType::Float1:
			case  VertexLayoutType::Float2:
			case  VertexLayoutType::Float3:
			case  VertexLayoutType::Float4:
				glEnableVertexAttribArray(layout_data.position);
				glVertexAttribPointer(layout_data.position
					, VertexLayout::getTypeLength(layout_data.type)
					, GL_FLOAT
					, layout_data.nomarlized ? GL_TRUE : GL_FALSE
					, layout_data.stride
					, (const void*)0);
			default:
				break;
			}
		}

		auto index_buffer = mVertex->getIndexBuffer();
		if (index_buffer != nullptr)
		{
			if (index_buffer->getBufferID() == 0)
			{
				GLBuildCMD_CreateIndexBuffer cmd(index_buffer);
				cmd.execute(graphcis);
			}
			else
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer->getBufferID());
			}
		}

		glBindVertexArray(0);

		mVertex->apply(vao);
	}

	//---------------------------------------
	//
	//	GLBuildCMD_VertexBuffer
	//
	GLBuildCMD_CreateVertexBuffer::GLBuildCMD_CreateVertexBuffer(VertexBuffer* vertexBuffer)
		: mVertexBuffer(vertexBuffer)
	{
		mVertexBuffer->addRef();
	}

	GLBuildCMD_CreateVertexBuffer::~GLBuildCMD_CreateVertexBuffer()
	{
		mVertexBuffer->subRef();
	}

	void GLBuildCMD_CreateVertexBuffer::execute(BaseGraphics* graphcis)
	{
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER
			, mVertexBuffer->getDataSize()
			, mVertexBuffer->getData()
			, mVertexBuffer->getData() == nullptr ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

		mVertexBuffer->apply(vbo);
	}


	//---------------------------------------
	//
	//	GLBuildCMD_CreateIndexBuffer
	//
	GLBuildCMD_CreateIndexBuffer::GLBuildCMD_CreateIndexBuffer(IndexBuffer* indexBuffer)
		: mIndexBuffer(indexBuffer)
	{
		mIndexBuffer->addRef();
	}

	GLBuildCMD_CreateIndexBuffer::~GLBuildCMD_CreateIndexBuffer()
	{
		mIndexBuffer->subRef();
	}

	void GLBuildCMD_CreateIndexBuffer::execute(BaseGraphics* graphcis)
	{
		GLuint ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER
			, mIndexBuffer->getDataSize()
			, mIndexBuffer->getData()
			, mIndexBuffer->getData() == nullptr ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		mIndexBuffer->apply(ebo);
	}


	//---------------------------------------
	//
	//	GLBuildCMD_Textrue2D
	//
	GLBuildCMD_CreateTextrue2D::GLBuildCMD_CreateTextrue2D(Texture2D* tex)
		: mTex(tex)
	{
		mTex->addRef();
	}

	GLBuildCMD_CreateTextrue2D::~GLBuildCMD_CreateTextrue2D()
	{
		mTex->subRef();
	}

	void GLBuildCMD_CreateTextrue2D::execute(BaseGraphics* graphcis)
	{
		GLuint tex_id;
		glGenTextures(1, &tex_id);
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glTexImage2D(GL_TEXTURE_2D
				   , 0
				   , mTex->getInternalChannel().platform
				   , mTex->getWidth(), mTex->getHeight()
				   , 0
				   , mTex->getChannel().platform
				   , mTex->getDataType().platform
				   , mTex->getData());

		if (mTex->getWrapS().tiny == TextureWrap::Clamp_To_Border
			|| mTex->getWrapT().tiny == TextureWrap::Clamp_To_Border)
		{
			float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mTex->getWrapS().platform);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mTex->getWrapT().platform);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mTex->getMinFilter().platform);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mTex->getMagFilter().platform);

		if (mTex->getMinFilter().tiny == TextureFilter::Linear_Mipmap_Linear
			|| mTex->getMinFilter().tiny == TextureFilter::Linear_Mipmap_Linear)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 5);
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		mTex->apply(tex_id);
	}

	//---------------------------------------
	//
	//	GLBuildCMD_TextrueCube
	//
	GLBuildCMD_CreateTextrueCube::GLBuildCMD_CreateTextrueCube(TextureCube* tex)
		: mTex(tex)
	{
		mTex->addRef();
	}

	GLBuildCMD_CreateTextrueCube::~GLBuildCMD_CreateTextrueCube()
	{
		mTex->subRef();
	}

	void GLBuildCMD_CreateTextrueCube::execute(BaseGraphics* graphcis)
	{
		GLuint tex_id;
		glGenTextures(1, &tex_id);
		TINY_GL_Check(glBindTexture(GL_TEXTURE_CUBE_MAP, tex_id));

		auto width = mTex->getWidth();
		auto height = mTex->getHeight();

		for (unsigned int i = 0; i < 6; i++)
		{
			TINY_GL_Check(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i
				, 0
				, mTex->getInternalChannel().platform
				, width, height
				, 0
				, mTex->getChannel().platform
				, mTex->getDataType().platform
				, mTex->getData(i)));
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, mTex->getWrapS().platform);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, mTex->getWrapT().platform);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, mTex->getWrapR().platform);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, mTex->getMinFilter().platform);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, mTex->getMagFilter().platform);

		if (mTex->getMinFilter().tiny == TextureFilter::Linear_Mipmap_Linear
			|| mTex->getMagFilter().tiny == TextureFilter::Linear_Mipmap_Linear)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 5);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		mTex->apply(tex_id);
	}

	//---------------------------------------
	//
	//	GLBuildCMD_CreateRender2D
	//
	GLBuildCMD_CreateRender2D::GLBuildCMD_CreateRender2D(TextureRender2D* tex)
		: mTex(tex)
	{
		mTex->addRef();
	}

	GLBuildCMD_CreateRender2D::~GLBuildCMD_CreateRender2D()
	{
		mTex->subRef();
	}

	void GLBuildCMD_CreateRender2D::execute(BaseGraphics* graphcis)
	{
		GLuint tex_id;
		glGenRenderbuffers(1, &tex_id);
		glBindRenderbuffer(GL_RENDERBUFFER, tex_id);
		glRenderbufferStorage(GL_RENDERBUFFER
			, mTex->getInternalChannel().platform
			, mTex->getWidth()
			, mTex->getHeight());
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		mTex->apply(tex_id);
	}

	//---------------------------------------
	//
	//	GLBuildCMD_CreateFrameBuffer
	//
	GLBuildCMD_CreateFrameBuffer::GLBuildCMD_CreateFrameBuffer(FrameBuffer* frameBuffer)
		: mFrameBuffer(frameBuffer)
		, mColorCount(0)
	{
		mFrameBuffer->addRef();
	}

	GLBuildCMD_CreateFrameBuffer::~GLBuildCMD_CreateFrameBuffer()
	{
		mFrameBuffer->subRef();
	}

	void GLBuildCMD_CreateFrameBuffer::execute(BaseGraphics* graphcis)
	{
		GLuint fbo;
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		auto& attachmentes = mFrameBuffer->getAttachmentes();
		for (auto i = 0; i < attachmentes.size(); i++)
		{
			auto tex = attachmentes[i];
			switch (tex->getTextureType())
			{
			case TextureType::Texture2D:
			{
				if (tex->getTextureID() == 0)
				{
					GLBuildCMD_CreateTextrue2D cmd((Texture2D*)tex);
					cmd.execute(graphcis);
				}
				this->attach2D((Texture2D*)tex);
				break;
			}
			case TextureType::TextureCube:
			{
				if (tex->getTextureID() == 0)
				{
					GLBuildCMD_CreateTextrueCube cmd((TextureCube*)tex);
					cmd.execute(graphcis);
				}
				this->attachCube((TextureCube*)tex, 0, 0);
				break;
			}
			case TextureType::TextureRender2D:
			{
				if (tex->getTextureID() == 0)
				{
					GLBuildCMD_CreateRender2D cmd((TextureRender2D*)tex);
					cmd.execute(graphcis);
				}
				this->attachRender((TextureRender2D*)tex);
				break;
			}
			default:
				break;
			}
		}

		//如果没有ColorBuffer,需要关闭颜色通道
		if (mColorCount < 1)
		{
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
		else if (mColorCount > 1)
		{
			GLuint* colors = new GLuint[mColorCount];
			for (uint32_t i = 0; i < mColorCount; i++)
			{
				colors[i] = GL_COLOR_ATTACHMENT0 + i;
			}
			glDrawBuffers(mColorCount, colors);
			delete[] colors;
		}

		TinyThrow_Runtime(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE, "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		mFrameBuffer->apply(fbo);
	}

	void GLBuildCMD_CreateFrameBuffer::attach(Texture* tex)
	{
		switch (tex->getTextureType())
		{
		case TextureType::Texture2D:
			this->attach2D((Texture2D*)tex);
			break;
		case TextureType::TextureCube:
			this->attachCube((TextureCube*)tex, 0, 0);
			break;
		case TextureType::TextureRender2D:
			this->attachRender((TextureRender2D*)tex);
			break;
		default:
			break;
		}
	}

	void GLBuildCMD_CreateFrameBuffer::attach2D(Texture2D* tex)
	{
		switch (tex->getAttachPosition())
		{
		case TextureAttachPosition::ColorComponent:
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER
								 , GL_COLOR_ATTACHMENT0 + mColorCount++
								 , GL_TEXTURE_2D
								 , tex->getTextureID()
								 , 0);
			break;
		}
		case TextureAttachPosition::DepthComponent:
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER
								 , GL_DEPTH_ATTACHMENT
								 , GL_TEXTURE_2D
								 , tex->getTextureID()
								 , 0);
			break;
		}
		case TextureAttachPosition::StencilCompoent:
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER
								 , GL_STENCIL_ATTACHMENT
								 , GL_TEXTURE_2D
								 , tex->getTextureID()
								 , 0);
			break;
		}
		case TextureAttachPosition::DepthStencilComponent:
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER
								 , GL_DEPTH_STENCIL_ATTACHMENT
								 , GL_TEXTURE_2D
								 , tex->getTextureID()
								 , 0);
			break;
		}
		default:
			break;
		}
	}

	void GLBuildCMD_CreateFrameBuffer::attachRender(TextureRender2D* tex)
	{
		switch (tex->getAttachPosition())
		{
		case TextureAttachPosition::DepthComponent:
		{
			glFramebufferRenderbuffer(GL_FRAMEBUFFER
									, GL_DEPTH_ATTACHMENT
									, GL_RENDERBUFFER
									, tex->getTextureID());
			break;
		}
		case TextureAttachPosition::StencilCompoent:
		{
			glFramebufferRenderbuffer(GL_FRAMEBUFFER
									, GL_STENCIL_ATTACHMENT
									, GL_RENDERBUFFER
									, tex->getTextureID());
			break;
		}
		case TextureAttachPosition::DepthStencilComponent:
		{
			glFramebufferRenderbuffer(GL_FRAMEBUFFER
									, GL_DEPTH_STENCIL_ATTACHMENT
									, GL_RENDERBUFFER
									, tex->getTextureID());
			break;
		}
		default:
			break;
		}
	}

	void GLBuildCMD_CreateFrameBuffer::attachCube(TextureCube* tex)
	{
		mColorCount = 6;
		for (uint32_t i = 0; i < mColorCount; i++)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER
								 , GL_COLOR_ATTACHMENT0 + i
								 , GL_TEXTURE_CUBE_MAP_POSITIVE_X + i
								 , tex->getTextureID()
								 , 0);
		}
	}

	void GLBuildCMD_CreateFrameBuffer::attachCube(TextureCube* tex, int colorIndex, int faceIndex, int level)
	{
		mColorCount = 1;
		glFramebufferTexture2D(GL_FRAMEBUFFER
							 , GL_COLOR_ATTACHMENT0 + colorIndex
							 , GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex
							 , tex->getTextureID()
							 , level);
	}




	//---------------------------------------
	//
	//	GLBuildCMD_CreateShader
	//
	GLBuildCMD_CreateShader::GLBuildCMD_CreateShader(Shader* shader)
		: mShader(shader)
		, mContent()
	{
		mShader->addRef();
	}

	GLBuildCMD_CreateShader::GLBuildCMD_CreateShader(Shader* shader, std::string& content)
		: mShader(shader)
		, mContent(content)
	{
		mShader->addRef();
	}

	GLBuildCMD_CreateShader::~GLBuildCMD_CreateShader()
	{
		mShader->subRef();
	}

	void GLBuildCMD_CreateShader::execute(BaseGraphics* graphcis)
	{
		if (mContent.empty())
		{
			mContent = FileTool::loadText(mShader->getFilePath());

			if (mContent.empty())
			{
				return;
			}
		}

		auto root = std::filesystem::path(mShader->getFilePath()).parent_path().string();

		GLShaderBuilder builder;
		builder.parseHeader(mContent);
		builder.parseShaders(mContent, root);
		builder.updateShaderConfig(mShader);

		auto& vertex = builder.getVertexShader();
		auto& frag = builder.getFragShader();

		auto pid = glCreateProgram();
		this->build(pid, vertex.c_str(), GL_VERTEX_SHADER);
		this->build(pid, frag.c_str(), GL_FRAGMENT_SHADER);

		glLinkProgram(pid);

		int success;
		char infoLog[512];
		glGetProgramiv(pid, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(pid, 512, nullptr, infoLog);
			Log_Error(mShader->getName() + "|" + infoLog);
			return;
		}

		std::function<void(ArgMetaData*, const std::string&)> progress_tiny =
			[this, &progress_tiny, &pid](ArgMetaData* metaData, const std::string& parentName)
		{
			auto name = metaData->valueName;
			auto array_size = metaData->valueCount;
			auto is_root = parentName.empty();

			//如果是类,需要拼接名称
			if (metaData->valueType == UniformType::Struct)
			{
				auto& members = metaData->getInfo<ArgStructInfo>()->members;

				if (array_size > 0)
				{
					for (uint32_t i = 0; i < array_size; i++)
					{
						for (auto& m : members)
						{
							std::string true_name = is_root ? fmt::format("{}[{}]", name, i) : fmt::format("{}.{}[{}]", parentName, name, i);
							progress_tiny(m.get(), true_name);
						}
					}
				}
				else
				{
					for (auto& m : members)
					{
						std::string true_name = is_root ? name : fmt::format("{}.{}", parentName, name);
						progress_tiny(m.get(), true_name);
					}
				}
			}
			else
			{
				if (array_size > 0)
				{
					for (uint32_t i = 0; i < array_size; i++)
					{
						std::string true_name = is_root ? fmt::format("{}[{}]", name, i) : fmt::format("{}.{}[{}]", parentName, name, i);
						auto uid = UniformID::getUIDStatic(true_name);
						if (uid < mShader->getTinyUniformCount())
						{
							mShader->setupTinyUniform(metaData, true_name, uid, glGetUniformLocation(pid, true_name.c_str()), i);
						}
						else
						{
							Log_Error(fmt::format("Your Shader`s buildin value name[{}] write error!!!", true_name));
						}
					}
				}
				else
				{
					std::string true_name = is_root ? name : fmt::format("{}.{}", parentName, name);
					auto uid = UniformID::getUIDStatic(true_name);
					if (uid < mShader->getTinyUniformCount())
					{
						mShader->setupTinyUniform(metaData, true_name, uid, glGetUniformLocation(pid, true_name.c_str()));
					}
					else
					{
						Log_Error(fmt::format("Your Shader`s buildin value name[{}] write error!!!", true_name));
					}
				}
			}
		};

		std::function<void(ArgMetaData*, const std::string&)> progress_user =
			[this, &progress_user, &pid](ArgMetaData* metaData, const std::string& parentName)
		{
			auto& name = metaData->valueName;
			auto array_size = metaData->valueCount;
			auto is_root = parentName.empty();

			//如果是类,需要拼接名称
			if (metaData->valueType == UniformType::Struct)
			{
				auto& members = metaData->getInfo<ArgStructInfo>()->members;

				if (array_size > 0)
				{
					for (uint32_t i = 0; i < array_size; i++)
					{
						for (auto& m : members)
						{
							std::string true_name = is_root ? fmt::format("{}[{}]", name, i) : fmt::format("{}.{}[{}]", parentName, name, i);
							progress_user(m.get(), true_name);
						}
					}
				}
				else
				{
					for (auto& m : members)
					{
						std::string true_name = is_root ? name : fmt::format("{}.{}", parentName, name);
						progress_user(m.get(), true_name);
					}
				}
			}
			else
			{
				if (array_size > 0)
				{
					for (uint32_t i = 0; i < array_size; i++)
					{
						std::string true_name = is_root ? fmt::format("{}[{}]", name, i) : fmt::format("{}.{}[{}]", parentName, name, i);
						mShader->setupUserUniformID(metaData, true_name, glGetUniformLocation(pid, true_name.c_str()), i);
					}
				}
				else
				{
					std::string true_name = is_root ? name : fmt::format("{}.{}", parentName, name);
					mShader->setupUserUniformID(metaData, true_name, glGetUniformLocation(pid, true_name.c_str()));
				}
			}
		};

		mShader->resizeTinyUniformAry(UniformID::allStringCount());
		for (auto& pair : builder.mTinyUMap)
		{
			progress_tiny(pair.second.get(), "");
		}

		for (auto& pair : builder.mUserUMap)
		{
			progress_user(pair.second.get(), "");
		}


		mShader->apply(pid);

		Log_Engine(fmt::format("Shader[{}] Build Complete[{}] ({})"
			, mShader->getName()
			, mShader->getProgramID()
			, mShader->getFilePath()));
	}

	void GLBuildCMD_CreateShader::build(uint32_t pid, const char* data, uint32_t shaderType)
	{
		auto shader_id = glCreateShader(shaderType);
		glShaderSource(shader_id, 1, &data, nullptr);
		glCompileShader(shader_id);

		int success;
		char infoLog[512];
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader_id, 512, nullptr, infoLog);
			switch (shaderType)
			{
			case GL_VERTEX_SHADER:
				std::cout << "GLShader [" + mShader->getName() + "]: [VERTEX] COMPILATION_FAILED > " << infoLog << std::endl;
				Log_Error(fmt::format("GLShader[{}]: [VERTEX] COMPILATION_FAILED > {})", mShader->getName(), infoLog));
				break;
			case GL_FRAGMENT_SHADER:
				std::cout << "GLShader [" + mShader->getName() + "]: [FRAGMENT] COMPILATION_FAILED > " << infoLog << std::endl;
				Log_Error(fmt::format("GLShader[{}]: [FRAGMENT] COMPILATION_FAILED > {})", mShader->getName(), infoLog));
				break;
			default:
				break;
			}
		}

		glAttachShader(pid, shader_id);
		glDeleteShader(shader_id);
	}
#pragma endregion

#pragma region Update
	GLBuildCMD_UpdateTextrue2D::GLBuildCMD_UpdateTextrue2D(Texture2D* tex)
		: mTex(tex)
	{
		mTex->addRef();
	}

	GLBuildCMD_UpdateTextrue2D::~GLBuildCMD_UpdateTextrue2D()
	{
		mTex->subRef();
	}

	void GLBuildCMD_UpdateTextrue2D::execute(BaseGraphics* graphcis)
	{
		glBindTexture(GL_TEXTURE_2D, mTex->getTextureID());
		glTexSubImage2D(GL_TEXTURE_2D
					   , 0
					   , 0, 0
					   , mTex->getWidth(), mTex->getHeight()
					   , mTex->getChannel().platform
					   , mTex->getDataType().platform
					   , mTex->getData());
		glBindTexture(GL_TEXTURE_2D, 0);

		mTex->apply(mTex->getTextureID());
	}
#pragma endregion


#pragma region Delete

	void GLBuildCMD_DeleteVAO::execute(BaseGraphics* graphcis)
	{
		glDeleteVertexArrays(1, &mID);
	}

	void GLBuildCMD_DeleteVBO::execute(BaseGraphics* graphcis)
	{
		glDeleteBuffers(1, &mID);
	}

	void GLBuildCMD_DeleteIBO::execute(BaseGraphics* graphcis)
	{
		glDeleteBuffers(1, &mID);
	}

	void GLBuildCMD_DeleteTexture::execute(BaseGraphics* graphcis)
	{
		glDeleteTextures(1, &mID);
	}

	void GLBuildCMD_DeleteRender2D::execute(BaseGraphics* graphcis)
	{
		glDeleteRenderbuffers(1, &mID);
	}

	void GLBuildCMD_DeleteFrameBuffer::execute(BaseGraphics* graphcis)
	{
		glDeleteFramebuffers(1, &mID);
	}

	void GLBuildCMD_DeleteShader::execute(BaseGraphics* graphcis)
	{
		glDeleteProgram(mID);
	}
#pragma endregion



}
