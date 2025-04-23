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

#include "Platform/OpenGL/GLFunction.h"
#include "Platform/OpenGL/GLHead.h"

#include "Core/Data/Image.h"

#include "Core/Renderer/Texture.h"
#include "Core/Renderer/FrameBuffer.h"
#include "Core/Renderer/Vertex.h"
#include "Core/Renderer/VertexBuffer.h"

#include "Core/Head/TinyCpp.h"
#include "Core/Debug/Debug.h"


#include "Core/Shader/Shader.h"
#include "core/Shader/ShaderParser.h"

#include "Core/Manager/LightingManager.h"
#include "Core/Manager/VertexBufferManager.h"

namespace tezcat::Tiny::GL
{
	GLFunction_Buffer GLFunction::FuncBuffer;
	GLFunction_FrameBuffer GLFunction::FuncFrameBuffer;
	GLFunction_Shader GLFunction::FuncShader;
	GLFunction_Texture GLFunction::FuncTexture;
	GLFunction_PBR GLFunction::FuncPBR;

	//---------------------------------------
	//
	//	Create Vertex
	//
	void GLFunction_Buffer::build(Vertex* vertex)
	{
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		for (auto buffer : vertex->getVertexBuffers())
		{
			if (buffer->getBufferID() == 0)
			{
				this->build(buffer);
			}
			else
			{
				glBindBuffer(GL_ARRAY_BUFFER, buffer->getBufferID());
			}

			auto& layout_data = buffer->getLayoutData();
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
					, (GLsizei)layout_data.stride
					, (const void*)0);
				break;
			default:
				break;
			}
		}

		auto index_buffer = vertex->getIndexBuffer();
		if (index_buffer != nullptr)
		{
			if (index_buffer->getBufferID() == 0)
			{
				this->build(index_buffer);
			}
			else
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer->getBufferID());
			}

			//vertex->setIndexCount(index_buffer->getDataSize());
		}

		glBindVertexArray(0);

		vertex->apply(vao);
	}

	void GLFunction_Buffer::build(VertexBuffer* vertexBuffer)
	{
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER
			, vertexBuffer->getDataSize()
			, vertexBuffer->getData()
			, vertexBuffer->getData() == nullptr ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

		vertexBuffer->apply(vbo);
	}

	void GLFunction_Buffer::build(IndexBuffer* indexBuffer)
	{
		GLuint ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER
			, indexBuffer->getDataSize()
			, indexBuffer->getData()
			, indexBuffer->getData() == nullptr ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

		indexBuffer->apply(ebo);
	}

	void GLFunction_Buffer::build(UniformBuffer* uniformBuffer, int32_t bindingIndex)
	{
		GLuint ubo;
		glGenBuffers(1, &ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferData(GL_UNIFORM_BUFFER
			, uniformBuffer->getDataSize()
			, uniformBuffer->getData()
			, GL_STATIC_DRAW);

		if (bindingIndex > -1)
		{
			glBindBufferBase(GL_UNIFORM_BUFFER, bindingIndex, ubo);
		}

		uniformBuffer->apply(ubo);
	}

	//---------------------------------------
	//
	//	Create FrameBuffer
	//
	void GLFunction_FrameBuffer::build(FrameBuffer* frameBuffer)
	{
		mColorCount = 0;
		GLuint fbo;
		TINY_GL_CHECK(glGenFramebuffers(1, &fbo));
		TINY_GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

		auto& attachmentes = frameBuffer->getAttachmentes();
		for (auto i = 0; i < attachmentes.size(); i++)
		{
			auto tex = attachmentes[i];
			switch (tex->getTextureType())
			{
			case TextureType::Texture2D:
			{
				if (tex->getTextureID() == 0)
				{
					GLFunction::FuncTexture.build((Texture2D*)tex);
				}
				this->attach2D((Texture2D*)tex);
				break;
			}
			case TextureType::TextureCube:
			{
				if (tex->getTextureID() == 0)
				{
					GLFunction::FuncTexture.build((TextureCube*)tex);
				}
				this->attachCube((TextureCube*)tex, 0, 0);
				break;
			}
			case TextureType::TextureRender2D:
			{
				if (tex->getTextureID() == 0)
				{
					GLFunction::FuncTexture.build((TextureRender2D*)tex);
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
			TINY_GL_CHECK(glDrawBuffer(GL_NONE));
			TINY_GL_CHECK(glReadBuffer(GL_NONE));
		}
		else if (mColorCount > 1)
		{
			GLuint* colors = new GLuint[mColorCount];
			for (uint32_t i = 0; i < mColorCount; i++)
			{
				colors[i] = GL_COLOR_ATTACHMENT0 + i;
			}
			TINY_GL_CHECK(glDrawBuffers(mColorCount, colors));
			delete[] colors;
		}

		TINY_THROW_RUNTIME(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE, "ERROR::FRAMEBUFFER::Framebuffer is not complete!");
		TINY_GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));

		frameBuffer->apply(fbo);
	}

	void GLFunction_FrameBuffer::attach(Texture* tex)
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

	void GLFunction_FrameBuffer::attach2D(Texture2D* tex)
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

	void GLFunction_FrameBuffer::attachRender(TextureRender2D* tex)
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

	void GLFunction_FrameBuffer::attachCube(TextureCube* tex)
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

	void GLFunction_FrameBuffer::attachCube(TextureCube* tex, int colorIndex, int faceIndex, int level)
	{
		mColorCount = 1;
		TINY_GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER
			, GL_COLOR_ATTACHMENT0 + colorIndex
			, GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex
			, tex->getTextureID()
			, level));
	}


	//---------------------------------------
	//
	//	Create Shader
	//
	void GLFunction_Shader::build(Shader* shader)
	{
		std::unique_ptr<ShaderParser> parser(std::move(shader->mParser));

		auto& vertex = parser->mVertexShader;
		auto& frag = parser->mFragShader;
		auto& geometry = parser->mGeometryShader;

		auto pid = glCreateProgram();
		this->build(shader, pid, vertex.c_str(), GL_VERTEX_SHADER);
		this->build(shader, pid, frag.c_str(), GL_FRAGMENT_SHADER);
		if (!geometry.empty())
		{
			this->build(shader, pid, geometry.c_str(), GL_GEOMETRY_SHADER);
		}

		glLinkProgram(pid);

		int success;
		char infoLog[512];
		glGetProgramiv(pid, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(pid, 512, nullptr, infoLog);
			TINY_LOG_ERROR(shader->getName() + "|" + infoLog);
			TINY_ASSERT(false);
			return;
		}

		std::function<void(ShaderUniformMember*, const std::string&)> progress_tiny =
			[&progress_tiny, &pid, &shader](ShaderUniformMember* memberData, const std::string& parentName)
			{
				auto& name = memberData->valueName;
				auto array_size = memberData->valueCount;
				auto is_root = parentName.empty();

				//如果是类,需要拼接名称
				if (memberData->valueType == UniformType::Struct)
				{
					auto& members = memberData->getInfo<ShaderStructInfo>()->members;

					if (array_size > 0)
					{
						for (int32_t i = 0; i < array_size; i++)
						{
							for (auto& m : members)
							{
								std::string true_name = is_root ? std::format("{}[{}]", name, i) : std::format("{}.{}[{}]", parentName, name, i);
								progress_tiny(m.get(), true_name);
							}
						}
					}
					else
					{
						for (auto& m : members)
						{
							std::string true_name = is_root ? name : std::format("{}.{}", parentName, name);
							progress_tiny(m.get(), true_name);
						}
					}
				}
				else
				{
					if (array_size > 0)
					{
						for (int32_t i = 0; i < array_size; i++)
						{
							std::string true_name = is_root ? std::format("{}[{}]", name, i) : std::format("{}.{}[{}]", parentName, name, i);
							shader->setTinyUniform(true_name, glGetUniformLocation(pid, true_name.c_str()));

							//auto uid = UniformID::staticGetUID(true_name);
							//if (uid < shader->getTinyUniformCount())
							//{
							//}
							//else
							//{
							//	TINY_LOG_ERROR(std::format("Tiny Shader`s buildin value name[{}] write error!!!", true_name));
							//}
						}
					}
					else
					{
						std::string true_name = is_root ? name : std::format("{}.{}", parentName, name);
						shader->setTinyUniform(true_name, glGetUniformLocation(pid, true_name.c_str()));


						//auto uid = UniformID::staticGetUID(true_name);
						//if (uid < shader->getTinyUniformCount())
						//{
						//	shader->setTinyUniform(memberData, true_name, uid, glGetUniformLocation(pid, true_name.c_str()));
						//}
						//else
						//{
						//	TINY_LOG_ERROR(std::format("Tiny Shader`s buildin value name[{}] write error!!!", true_name));
						//}
					}
				}
			};

		std::function<void(ShaderUniformMember*, const std::string&)> progress_user =
			[&progress_user, &pid, &shader](ShaderUniformMember* memberData, const std::string& parentName)
			{
				auto& name = memberData->valueName;
				auto array_size = memberData->valueCount;
				auto is_root = parentName.empty();

				//如果是类,需要拼接名称
				if (memberData->valueType == UniformType::Struct)
				{
					auto& members = memberData->getInfo<ShaderStructInfo>()->members;

					if (array_size > 0)
					{
						for (int32_t i = 0; i < array_size; i++)
						{
							for (auto& m : members)
							{
								std::string true_name = is_root ? std::format("{}[{}]", name, i) : std::format("{}.{}[{}]", parentName, name, i);
								progress_user(m.get(), true_name);
							}
						}
					}
					else
					{
						for (auto& m : members)
						{
							std::string true_name = is_root ? name : std::format("{}.{}", parentName, name);
							progress_user(m.get(), true_name);
						}
					}
				}
				else
				{
					if (array_size > 0)
					{
						for (int32_t i = 0; i < array_size; i++)
						{
							std::string true_name = is_root ? std::format("{}[{}]", name, i) : std::format("{}.{}[{}]", parentName, name, i);
							shader->setUserUniform(true_name, glGetUniformLocation(pid, true_name.c_str()));
						}
					}
					else
					{
						std::string true_name = is_root ? name : std::format("{}.{}", parentName, name);
						shader->setUserUniform(true_name, glGetUniformLocation(pid, true_name.c_str()));
					}
				}
			};

		for (auto& pair : parser->mTinyUMap)
		{
			progress_tiny(pair.second.get(), "");
		}

		for (auto& pair : parser->mUserUMap)
		{
			progress_user(pair.second.get(), "");
		}

		//把shader连接到UBO槽上
		for (auto& pair : parser->mUBOMap)
		{
			auto info = VertexBufferManager::getUniformBufferLayout(pair.first);

			TINY_GL_CHECK_RETURN(glGetUniformBlockIndex(pid, pair.first.c_str()), ubID);
			TINY_GL_CHECK(glUniformBlockBinding(pid, ubID, info->mBindingIndex));

			if (!info->isGPUChecked())
			{
				int32_t block_size = 0;
				TINY_GL_CHECK(glGetActiveUniformBlockiv(pid, ubID, GL_UNIFORM_BLOCK_DATA_SIZE, &block_size));
				TINY_LOG_ENGINE(pair.first + std::to_string(block_size));
				info->mSize = block_size;
				
				for (auto& slot : info->mSlot)
				{
					uint32_t index;
					int32_t offset;
					auto name = slot.name.c_str();
					TINY_GL_CHECK(glGetUniformIndices(pid, 1, &name, &index));
					TINY_GL_CHECK(glGetActiveUniformsiv(pid, 1, &index, GL_UNIFORM_OFFSET, &offset));

					slot.offset = offset;
				}

				info->gpuCheckComplete();

				info->notifyHolderUpdateData();
			}
		}

		shader->apply(pid);

		TINY_LOG_ENGINE(std::format("Shader[{}] Build Complete[{}] ({})"
			, shader->getName()
			, shader->getProgramID()
			, shader->getFilePath()));
	}

	void GLFunction_Shader::build(Shader* shader, uint32_t pid, const char* data, uint32_t shaderType)
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
				TINY_LOG_ERROR(std::format("GLShader[{}]: [VERTEX] COMPILATION_FAILED > {}", shader->getName(), infoLog));
				break;
			case GL_FRAGMENT_SHADER:
				TINY_LOG_ERROR(std::format("GLShader[{}]: [FRAGMENT] COMPILATION_FAILED > {}", shader->getName(), infoLog));
				break;
			case GL_GEOMETRY_SHADER:
				TINY_LOG_ERROR(std::format("GLShader[{}]: [GEOMETRY] COMPILATION_FAILED > {}", shader->getName(), infoLog));
				break;
			default:
				break;
			}
		}

		glAttachShader(pid, shader_id);
		glDeleteShader(shader_id);
	}


	//---------------------------------------
	//
	//	Create Texture
	//
	void GLFunction_Texture::build(Texture2D* tex2D)
	{
		GLuint tex_id;
		TINY_GL_CHECK(glGenTextures(1, &tex_id));
		TINY_GL_CHECK(glBindTexture(GL_TEXTURE_2D, tex_id));
		TINY_GL_CHECK(glTexImage2D(GL_TEXTURE_2D
			, 0
			, tex2D->getInternalFormat().platform
			, tex2D->getWidth(), tex2D->getHeight()
			, 0
			, tex2D->getFormat().platform
			, tex2D->getDataMemFormat().platform
			, tex2D->getData()));

		if (tex2D->getWrapS().tiny == TextureWrap::Clamp_To_Border
			|| tex2D->getWrapT().tiny == TextureWrap::Clamp_To_Border)
		{
			float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			TINY_GL_CHECK(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));
		}

		TINY_GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tex2D->getWrapS().platform));
		TINY_GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tex2D->getWrapT().platform));

		TINY_GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tex2D->getMinFilter().platform));
		TINY_GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tex2D->getMagFilter().platform));

		if (tex2D->getMinFilter().tiny == TextureFilter::Linear_Mipmap_Linear)
		{
			TINY_GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0));
			TINY_GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 5));
			TINY_GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
		}

		TINY_GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
		tex2D->apply(tex_id);
	}

	void GLFunction_Texture::build(TextureRender2D* render2D)
	{
		GLuint tex_id;
		TINY_GL_CHECK(glGenRenderbuffers(1, &tex_id));
		TINY_GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, tex_id));
		TINY_GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER
			, render2D->getInternalFormat().platform
			, render2D->getWidth()
			, render2D->getHeight()));
		TINY_GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		render2D->apply(tex_id);
	}

	void GLFunction_Texture::build(TextureCube* texCube)
	{
		GLuint tex_id;
		glGenTextures(1, &tex_id);
		TINY_GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, tex_id));

		auto size = texCube->getSize();

		for (uint32_t i = 0; i < 6; i++)
		{
			TINY_GL_CHECK(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i
				, 0
				, texCube->getInternalFormat().platform
				, size, size
				, 0
				, texCube->getFormat().platform
				, texCube->getDataMemFormat().platform
				, texCube->getData(i)));
		}

		TINY_GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, texCube->getWrapS().platform));
		TINY_GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, texCube->getWrapT().platform));
		TINY_GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, texCube->getWrapR().platform));

		TINY_GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, texCube->getMinFilter().platform));
		TINY_GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, texCube->getMagFilter().platform));

		if (texCube->getMinFilter().tiny == TextureFilter::Linear_Mipmap_Linear)
		{
			TINY_GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0));
			TINY_GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 5));
			TINY_GL_CHECK(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
		}

		TINY_GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));

		texCube->apply(tex_id);
	}

	void GLFunction_Texture::update(Texture2D* tex2D)
	{
		glBindTexture(GL_TEXTURE_2D, tex2D->getTextureID());
		glTexSubImage2D(GL_TEXTURE_2D
					   , 0
					   , 0, 0
					   , tex2D->getWidth(), tex2D->getHeight()
					   , tex2D->getFormat().platform
					   , tex2D->getDataMemFormat().platform
					   , tex2D->getData());
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	GLFunction_PBR::GLFunction_PBR()
	{
		mCaptureViews[0] = glm::lookAt(float3(0.0f, 0.0f, 0.0f), float3(1.0f, 0.0f, 0.0f), float3(0.0f, -1.0f, 0.0f));
		mCaptureViews[1] = glm::lookAt(float3(0.0f, 0.0f, 0.0f), float3(-1.0f, 0.0f, 0.0f), float3(0.0f, -1.0f, 0.0f));
		mCaptureViews[2] = glm::lookAt(float3(0.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f), float3(0.0f, 0.0f, 1.0f));
		mCaptureViews[3] = glm::lookAt(float3(0.0f, 0.0f, 0.0f), float3(0.0f, -1.0f, 0.0f), float3(0.0f, 0.0f, -1.0f));
		mCaptureViews[4] = glm::lookAt(float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 1.0f), float3(0.0f, -1.0f, 0.0f));
		mCaptureViews[5] = glm::lookAt(float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, -1.0f), float3(0.0f, -1.0f, 0.0f));
	}

	//-------------------------------------------------------
	//
	//	RenderCMD_HDRToCube
	//
	void GLFunction_PBR::makeHDR2Cube(Shader* shader
		, Vertex* vertex
		, Texture2D* texHDR
		, TextureCube* skybox) const
	{
		shader->resetLocalState();
		auto uniform_config = shader->getUserUniformValueConfig("myTexHDR2D");
		Graphics::getInstance()->setTexture2D(shader, uniform_config->valueID, texHDR);

		auto array = LightingManager::getCubeMapTextureArray();

		for (uint32_t i = 0; i < 6; i++)
		{
			Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixV, mCaptureViews[i]);

			glFramebufferTexture2D(GL_FRAMEBUFFER
				 , GL_COLOR_ATTACHMENT0
				 , GL_TEXTURE_2D
				 , array[i]->getTextureID()
				 , 0);

			Graphics::getInstance()->clear(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
			Graphics::getInstance()->draw(vertex);

			glFramebufferTexture2D(GL_FRAMEBUFFER
				, GL_COLOR_ATTACHMENT0
				, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i
				, skybox->getTextureID()
				, 0);

			Graphics::getInstance()->clear(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
			Graphics::getInstance()->draw(vertex);
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getTextureID());
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 5);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void GLFunction_PBR::makeEnvIrradiance(Shader* shader
		, Vertex* vertex
		, TextureCube* skybox
		, TextureCube* irradiance) const
	{
		shader->resetLocalState();
		Graphics::getInstance()->setTextureCube(shader, ShaderParam::TexSkybox, skybox);

		for (uint32_t i = 0; i < 6; i++)
		{
			//mFrameBuffer->attachCube(mIrradianceMap, 0, i);
			glFramebufferTexture2D(GL_FRAMEBUFFER
								 , GL_COLOR_ATTACHMENT0
								 , GL_TEXTURE_CUBE_MAP_POSITIVE_X + i
								 , irradiance->getTextureID()
								 , 0);

			Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixV, mCaptureViews[i]);
			//shader->setViewMatrix(captureViews[i]);

			Graphics::getInstance()->clear(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
			Graphics::getInstance()->draw(vertex);
		}
	}

	void GLFunction_PBR::makeEnvPrefilter(Shader* shader
		, Vertex* vertex
		, TextureCube* skybox
		, TextureCube* prefitler
		, uint32_t mipMaxLevel
		, uint32_t mipWidth
		, uint32_t mipHeight
		, int32_t resolution) const
	{
		shader->resetLocalState();
		Graphics::getInstance()->setTextureCube(shader, ShaderParam::TexSkybox, skybox);

		for (uint32_t mip = 0; mip < mipMaxLevel; ++mip)
		{
			uint32_t mip_width = static_cast<uint32_t>(mipWidth * std::pow(0.5, mip));
			uint32_t mip_height = static_cast<uint32_t>(mipHeight * std::pow(0.5, mip));
			Graphics::getInstance()->setViewport(ViewportInfo(0, 0, mip_width, mip_height));

			float roughness = (float)mip / (float)(mipMaxLevel - 1);
			Graphics::getInstance()->setFloat1(shader, "myRoughness", &roughness);
			Graphics::getInstance()->setInt1(shader, "myResolution", &resolution);

			for (uint32_t i = 0; i < 6; ++i)
			{
				//mFrameBuffer->attachCube(mPrefilterMap, 0, i, mip);
				glFramebufferTexture2D(GL_FRAMEBUFFER
									 , GL_COLOR_ATTACHMENT0
									 , GL_TEXTURE_CUBE_MAP_POSITIVE_X + i
									 , prefitler->getTextureID()
									 , mip);

				Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixV, mCaptureViews[i]);

				Graphics::getInstance()->clear(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
				Graphics::getInstance()->draw(vertex);
			}
		}
	}
}
