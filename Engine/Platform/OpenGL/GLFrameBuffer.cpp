#include "GLFrameBuffer.h"
#include "GLTexture.h"
#include "Core/Renderer/Texture.h"

namespace tezcat::Tiny::GL
{
	TINY_RTTI_CPP(GLFrameBuffer)
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
		glBindFramebuffer(GL_FRAMEBUFFER, mBufferID);
	}

// 	void GLFrameBuffer::unbind()
// 	{
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//  {

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

		if (mColorCount > 1)
		{
			GLuint* colors = new GLuint[mColorCount];
			for (uint32_t i = 0; i < mColorCount; i++)
			{
				colors[i] = GL_COLOR_ATTACHMENT0 + i;
			}
			glDrawBuffers(mColorCount, colors);
			delete[] colors;
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
		return GLFrameBuffer::create();
	}

	GLFrameBufferCreator::GLFrameBufferCreator()
	{
		FrameBuffer::setDefaultBuffer(GLFrameBufferDefault::create());
	}


	//------------------------------------------------------
	//
	//
	//
	TINY_RTTI_CPP(GLFrameBufferDefault)
	GLFrameBufferDefault::GLFrameBufferDefault()
	{
		mBufferID = 0;
	}

	GLFrameBufferDefault::~GLFrameBufferDefault()
	{

	}

	void GLFrameBufferDefault::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mBufferID);
	}

// 	void GLFrameBufferDefault::unbind()
// 	{
// 		throw std::logic_error("The method or operation is not implemented.");
// 	}

	void GLFrameBufferDefault::attach(TextureRenderBuffer2D* buffer)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLFrameBufferDefault::build(const std::function<void(FrameBuffer*) >& function)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}


}
