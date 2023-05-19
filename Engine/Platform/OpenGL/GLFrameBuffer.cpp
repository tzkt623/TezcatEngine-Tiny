#include "GLFrameBuffer.h"
#include "GLTexture.h"
#include "Core/Renderer/Texture.h"

namespace tezcat::Tiny::GL
{

	//------------------------------------------------------
	//
	// GLFrameBufferDefault
	//
	TINY_RTTI_CPP(GLFrameBufferDefault);
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

	void GLFrameBufferDefault::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	//------------------------------------------------------
	//
	// GLFrameBuffer
	//
	TINY_RTTI_CPP(GLFrameBuffer);
	GLFrameBuffer::GLFrameBuffer()
		: mColorCount(0)
		, mBuild(false)
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

	void GLFrameBuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GLFrameBuffer::beginBuild()
	{
		mBuild = true;
		glBindFramebuffer(GL_FRAMEBUFFER, mBufferID);
	}

	void GLFrameBuffer::endBuild()
	{
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

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Log_Error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		mBuild = false;
	}

	void GLFrameBuffer::attach2D(Texture2D* tex)
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

		if (mBuild)
		{
			mBuffers.push_back(tex);
		}
	}

	void GLFrameBuffer::attachRender(TextureRender2D* tex)
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

		if (mBuild)
		{
			mBuffers.push_back(tex);
		}
	}

	void GLFrameBuffer::attachCube(TextureCube* tex)
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

		if (mBuild)
		{
			mBuffers.push_back(tex);
		}
	}

	void GLFrameBuffer::attachCube(TextureCube* tex, int colorIndex, int faceIndex, int level)
	{
		mColorCount = 1;
		glFramebufferTexture2D(GL_FRAMEBUFFER
							 , GL_COLOR_ATTACHMENT0 + colorIndex
							 , GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex
							 , tex->getTextureID()
							 , level);

		if (mBuild)
		{
			mBuffers.push_back(tex);
		}
	}

	void GLFrameBuffer::attach(Texture* tex)
	{
		switch (tex->getTextureType())
		{
		case TextureType::Texture2D:
			this->attach2D((Texture2D*)tex);
			break;
		case TextureType::TextureCube:
			this->attachCube((TextureCube*)tex);
			break;
		case TextureType::TextureRender2D:
			this->attachRender((TextureRender2D*)tex);
			break;
		default:
			break;
		}
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
}
