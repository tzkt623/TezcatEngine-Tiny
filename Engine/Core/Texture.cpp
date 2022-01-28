#include "Texture.h"
#include "Image.h"


namespace tezcat::Tiny::Core
{
	Texture::Texture() :
		m_Filter(Filter::Tex_LINEAR),
		m_Wrap(Wrap::Tex_REPEAT),
		m_TexID(0)
	{

	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_TexID);
	}

	void Texture::createTexture(Image* image)
	{
		glGenTextures(1, &m_TexID);
		glBindTexture(GL_TEXTURE_2D, m_TexID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_Wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_Wrap);

		if (m_Wrap == Wrap::Tex_CLAMP_TO_BORDER)
		{
			float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_Filter);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->getWidth(), image->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}
