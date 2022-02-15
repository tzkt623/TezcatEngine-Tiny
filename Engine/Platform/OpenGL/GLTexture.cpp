#include "GLTexture.h"
#include "Core/Data/Image.h"


namespace tezcat::Tiny::Core
{
	//--------------------------------------------------------
	//
	//	Texture 2D
	//
	GLTexture2D::GLTexture2D()
	{
		glGenTextures(1, &m_TextureID);
	}

	GLTexture2D::~GLTexture2D()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void GLTexture2D::createTexture(Image* image)
	{
		GLenum source_format = GL_RGBA;
		switch (image->getChannels())
		{
		case 1:
			source_format = GL_R8;
			break;
		case 2:
			source_format = GL_RG;
			break;
		case 3:
			source_format = GL_RGB;
			break;
		case 4:
			source_format = GL_RGBA;
			break;
		default:
			break;
		}

		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ContextMap::TextureWrap[(uint32_t)m_Wrap]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ContextMap::TextureWrap[(uint32_t)m_Wrap]);

		if (m_Wrap == TextureWrap::Tex_CLAMP_TO_BORDER)
		{
			float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ContextMap::TextureFilter[(uint32_t)m_Filter]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ContextMap::TextureFilter[(uint32_t)m_Filter]);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->getWidth(), image->getHeight(), 0, source_format, GL_UNSIGNED_BYTE, image->getData());
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLTexture2D::createTexture(const std::string& filePath)
	{
		Image img;
		img.openFile(filePath);
		this->createTexture(&img);
	}
}