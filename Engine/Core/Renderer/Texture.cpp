#include "Texture.h"
#include "../Data/Image.h"


namespace tezcat::Tiny::Core
{
	Texture::Texture()
		: m_Filter(TexFilter::Tex_LINEAR)
		, m_Wrap(TexWrap::Tex_REPEAT)
		, m_TexID(0)
	{
		glGenTextures(1, &m_TexID);
//		glCreateTextures
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_TexID);
	}

	void Texture::createTexture(Image* image)
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

		glBindTexture(GL_TEXTURE_2D, m_TexID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_Wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_Wrap);

		if (m_Wrap == TexWrap::Tex_CLAMP_TO_BORDER)
		{
			float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)m_Filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)m_Filter);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->getWidth(), image->getHeight(), 0, source_format, GL_UNSIGNED_BYTE, image->getData());
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::createTexture(const std::string& filePath)
	{
		Image img;
		img.openFile(filePath);
		this->createTexture(&img);
	}
}
