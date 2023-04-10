#include "GLFrameBuffer.h"
#include "GLTexture.h"
#include "Core/Renderer/Texture.h"

namespace tezcat::Tiny::GL
{
	GLFrameBuffer::GLFrameBuffer()
		: mColorCount(0)
	{
		glGenFramebuffers(1, &mBufferID);
	}

	GLFrameBuffer::~GLFrameBuffer()
	{
		glDeleteFramebuffers(1, &mBufferID);
	}

	void GLFrameBuffer::bind()
	{
		if (mControlRenderSize)
		{
			glViewport(mViewportInfo.OX, mViewportInfo.OY, mViewportInfo.Width, mViewportInfo.Height);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, mBufferID);
	}

	void GLFrameBuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GLFrameBuffer::createTextureBuffer(TextureBuffer2D* tex)
	{
		switch (tex->getBufferType())
		{
		case TextureBufferType::ColorComponent:
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER
				, GL_COLOR_ATTACHMENT0 + mColorCount++
				, GL_TEXTURE_2D
				, tex->getTextureID()
				, 0);
			break;
		}
		case TextureBufferType::DepthComponent:
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER
				, GL_DEPTH_ATTACHMENT
				, GL_TEXTURE_2D
				, tex->getTextureID()
				, 0);
			break;
		}
		case TextureBufferType::StencilCompoent:
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER
				, GL_STENCIL_ATTACHMENT
				, GL_TEXTURE_2D
				, tex->getTextureID()
				, 0);
			break;
		}
		case TextureBufferType::DepthStencilComponent:
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

	void GLFrameBuffer::createRenderBuffer(TextureRenderBuffer2D* tex)
	{
		switch (tex->getBufferType())
		{
		case TextureBufferType::DepthComponent:
		{
			glFramebufferRenderbuffer(GL_FRAMEBUFFER
				, GL_DEPTH_ATTACHMENT
				, GL_RENDERBUFFER
				, tex->getTextureID());
			break;
		}
		case TextureBufferType::StencilCompoent:
		{
			glFramebufferRenderbuffer(GL_FRAMEBUFFER
				, GL_STENCIL_ATTACHMENT
				, GL_RENDERBUFFER
				, tex->getTextureID());
			break;
		}
		case TextureBufferType::DepthStencilComponent:
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

	void GLFrameBuffer::attach(TextureRenderBuffer2D* buffer)
	{
		if (buffer->getTextureType() == TextureType::TextureRenderBuffer2D)
		{
			this->createRenderBuffer(buffer);
		}
		else
		{
			this->createTextureBuffer((TextureBuffer2D*)buffer);
		}

		mBuffers.push_back(buffer);
	}

	void GLFrameBuffer::build(const std::function<void(FrameBuffer*) >& function)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mBufferID);

		function(this);

		//如果没有ColorBuffer,需要关闭颜色通道
		if (mColorCount < 1)
		{
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	//-----------------------------------------------------
	//
	//	Creator
	//
	FrameBuffer* GLFrameBufferCreator::createFrameBuffer()
	{
		return new GLFrameBuffer();
	}

}
