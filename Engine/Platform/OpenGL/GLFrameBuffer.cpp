#include "GLFrameBuffer.h"
#include "GLTexture.h"
#include "Core/Renderer/Texture.h"

namespace tezcat::Tiny::GL
{
	GLFrameBuffer::GLFrameBuffer()
	{
		glGenFramebuffers(1, &mBufferID);
	}

	GLFrameBuffer::~GLFrameBuffer()
	{

	}

	void GLFrameBuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mBufferID);
	}

	void GLFrameBuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GLFrameBuffer::create(TextureRenderBuffer2D* targetBuffer2D)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER
			, GL_COLOR_ATTACHMENT0
			, GL_TEXTURE_2D
			, targetBuffer2D->getTextureID()
			, 0);
	}

	void GLFrameBuffer::createTextureBuffer(const int& width, const int& high, const TextureBufferInfo& info, int& colorCountor)
	{
		auto tex = new GLTextureBuffer2D();
		tex->createTexture(width, high, info.internalChannel, info.channel, info.dataType);
		mBuffers.push_back(tex);

		switch (info.bufferType)
		{
		case TextureBufferType::Color:
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER
				, GL_COLOR_ATTACHMENT0 + colorCountor++
				, GL_TEXTURE_2D
				, tex->getTextureID()
				, 0);
			break;
		}
		case TextureBufferType::Depth:
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER
				, GL_DEPTH_ATTACHMENT
				, GL_TEXTURE_2D
				, tex->getTextureID()
				, 0);
			break;
		}
		case TextureBufferType::Stencil:
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER
				, GL_STENCIL_ATTACHMENT
				, GL_TEXTURE_2D
				, tex->getTextureID()
				, 0);
			break;
		}
		case TextureBufferType::DepthAndStencil:
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

	void GLFrameBuffer::createRenderBuffer(const int& width, const int& high, const TextureBufferInfo& info)
	{
		auto tex = new GLTextureRenderBuffer2D();
		tex->createTexture(width, high, info.internalChannel);
		mBuffers.push_back(tex);

		switch (info.bufferType)
		{
		case TextureBufferType::Depth:
		{
			glFramebufferRenderbuffer(GL_FRAMEBUFFER
				, GL_DEPTH_ATTACHMENT
				, GL_RENDERBUFFER
				, tex->getTextureID());
			break;
		}
		case TextureBufferType::Stencil:
		{
			glFramebufferRenderbuffer(GL_FRAMEBUFFER
				, GL_STENCIL_ATTACHMENT
				, GL_RENDERBUFFER
				, tex->getTextureID());
			break;
		}
		case TextureBufferType::DepthAndStencil:
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

	void GLFrameBuffer::create(const int& width, const int& high, const std::initializer_list<TextureBufferInfo>& infos)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mBufferID);

		int color_index = 0;
		for (const auto& item : infos)
		{
			if (item.isCache)
			{
				this->createRenderBuffer(width, high, item);
			}
			else
			{
				this->createTextureBuffer(width, high, item, color_index);
			}
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	FrameBuffer* GLFrameBufferCreator::create(const int& width, const int& high, const std::initializer_list<TextureBufferInfo>& infos)
	{
		auto buffer = new GLFrameBuffer();
		buffer->create(width, high, infos);
		return buffer;
	}
}
