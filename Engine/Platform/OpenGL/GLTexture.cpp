#include "GLTexture.h"
#include "Core/Data/Image.h"


namespace tezcat::Tiny::GL
{
	GLenum TINY_API getGLTextureChannels(Image* image)
	{
		switch (image->getChannels())
		{
		case 1: return GL_RED;
		case 2: return GL_RG;
		case 3: return GL_RGB;
		case 4: return GL_RGBA;
		default:
			break;
		}

		return GL_RGB;
	}

	//--------------------------------------------------------
	//
	//	Texture2D
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
		GLenum source_format = getGLTextureChannels(image);

		glBindTexture(GL_TEXTURE_2D, mTextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mWrap.platform);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mWrap.platform);

		if (mWrap.tiny == TextureWrap::Tex_CLAMP_TO_BORDER)
		{
			float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mFilter.platform);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mFilter.platform);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->getWidth(), image->getHeight(), 0, source_format, GL_UNSIGNED_BYTE, image->getData());
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//-------------------------------------------
	//
	//	Cube
	//
	GLTextureCube::GLTextureCube()
	{
		glGenTextures(1, &mTextureID);
	}

	GLTextureCube::~GLTextureCube()
	{
		glDeleteTextures(1, &mTextureID);
	}

	void GLTextureCube::createTexture(const std::string& filePath)
	{
		const char* dir[6] =
		{
			"R",
			"L",
			"U",
			"D",
			"F",
			"B"
		};

		glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);

		for (unsigned int i = 0; i < 6; i++)
		{
			auto path = StringTool::stringFormat(filePath, dir[i]);

			Image image;
			image.openFile(path, false);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i
						, 0
						, GL_RGB
						, image.getWidth(), image.getHeight()
						, 0
						, getGLTextureChannels(&image)
						, GL_UNSIGNED_BYTE
						, image.getData());
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, mFilter.platform);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, mFilter.platform);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, mWrap.platform);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, mWrap.platform);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, mWrap.platform);

		glBindTexture(GL_TEXTURE_2D, 0);
	}


	//----------------------------------------------
	//
	//	Creator
	//
	Texture* GLTextureCreator::create(const std::string& filePath, const TextureType& textureType, const TextureFilter& filter, const TextureWrap& wrap)
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
			texture = new GLTextureCube();
			break;
		case TextureType::Texture1DA:
			break;
		case TextureType::Texture2DA:
			break;
		default:
			break;
		}

		texture->createTexture(filePath);
		texture->setFilter(filter);
		texture->setWrap(wrap);
		return texture;
	}
}
