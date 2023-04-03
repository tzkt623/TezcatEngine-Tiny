#include "GLTexture.h"
#include "Core/Data/Image.h"


namespace tezcat::Tiny::GL
{
	//--------------------------------------------------------
	//
	//	Texture 2D
	//
	GLTexture2D::GLTexture2D()
	{
		glGenTextures(1, &mTextureID);
	}

	GLTexture2D::~GLTexture2D()
	{
		glDeleteTextures(1, &mTextureID);
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

		glBindTexture(GL_TEXTURE_2D, mTextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_Wrap.platform);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_Wrap.platform);

		if (m_Wrap.tiny == TextureWrap::Tex_CLAMP_TO_BORDER)
		{
			float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Filter.platform);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_Filter.platform);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->getWidth(), image->getHeight(), 0, source_format, GL_UNSIGNED_BYTE, image->getData());
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture* GLTextureCreator::create(const char* filePath, const TextureType& textureType)
	{
		Texture* texture = nullptr;
		switch (textureType)
		{
		case TextureType::Texture1D:
			break;
		case TextureType::Texture2D:
			texture = new GLTexture2D();
			break;
		case TextureType::Texture3D:
			break;
		case TextureType::TextureCube:
			break;
		case TextureType::Texture1DA:
			break;
		case TextureType::Texture2DA:
			break;
		default:
			break;
		}

		texture->createTexture(filePath);
		return texture;
	}
}
