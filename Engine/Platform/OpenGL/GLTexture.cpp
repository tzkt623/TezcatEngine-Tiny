#include "GLTexture.h"
#include "Core/Data/Image.h"
#include "Core/Head/Context.h"

using namespace tezcat::Tiny::Core;
namespace tezcat::Tiny::GL
{
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

	void GLTexture2D::create(const Image& image, const TextureInfo& info)
	{
		mWrap = ContextMap::TextureWrapArray[(uint32_t)info.wrap];
		mFilter = ContextMap::TextureFilterArray[(uint32_t)info.filter];
		mInternalChannel = ContextMap::TextureChannelArray[(uint32_t)info.internalChannel];
		mChannel = Texture::getTextureChannels(image);

		glBindTexture(GL_TEXTURE_2D, mTextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mWrap.platform);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mWrap.platform);

		if (mWrap.tiny == TextureWrap::Clamp_To_Border)
		{
			float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mFilter.platform);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mFilter.platform);

		glTexImage2D(GL_TEXTURE_2D
			, 0
			, mInternalChannel.platform
			, image.getWidth(), image.getHeight()
			, 0
			, mChannel.platform
			, mDataType.platform
			, image.getData());
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

	void GLTextureCube::create(const std::array<Image, 6>& images, const TextureInfo& info)
	{
		mFilter = ContextMap::TextureFilterArray[(uint32_t)info.filter];
		mWrap = ContextMap::TextureWrapArray[(uint32_t)info.wrap];

		glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);

		for (unsigned int i = 0; i < 6; i++)
		{
			auto& image = images[i];
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i
						, 0
						, Texture::getTextureChannels(image).platform
						, image.getWidth(), image.getHeight()
						, 0
						, Texture::getTextureChannels(image).platform
						, mDataType.platform
						, image.getData());
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, mFilter.platform);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, mFilter.platform);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, mWrap.platform);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, mWrap.platform);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, mWrap.platform);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//---------------------------------------------
	//
	//	Buffer2D
	//
	GLTextureBuffer2D::GLTextureBuffer2D()
	{
		glGenTextures(1, &mTextureID);
	}

	GLTextureBuffer2D::~GLTextureBuffer2D()
	{
		glDeleteTextures(1, &mTextureID);
	}

	void GLTextureBuffer2D::create(const int& width, const int& high, const TextureChannel& internalChannel)
	{

	}

	void GLTextureBuffer2D::create(const int& width, const int& high, const TextureChannel& internalChannel, const TextureChannel& channel, const DataType& dataType)
	{
		mInternalChannel = ContextMap::TextureChannelArray[(uint32_t)internalChannel];
		mChannel = ContextMap::TextureChannelArray[(uint32_t)channel];
		mDataType = ContextMap::DataTypeArray[(uint32_t)dataType];

		glBindTexture(GL_TEXTURE_2D, mTextureID);
		glTexImage2D(GL_TEXTURE_2D
			, 0
			, mInternalChannel.platform
			, width, high
			, 0
			, mChannel.platform
			, mDataType.platform
			, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mFilter.platform);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mFilter.platform);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLTextureBuffer2D::create(const int& width, const int& high, const TextureBufferInfo& info)
	{
		mWrap = ContextMap::TextureWrapArray[(uint32_t)info.wrap];
		mFilter = ContextMap::TextureFilterArray[(uint32_t)info.filter];
		mInternalChannel = ContextMap::TextureChannelArray[(uint32_t)info.internalChannel];
		mChannel = ContextMap::TextureChannelArray[(uint32_t)info.channel];

		glBindTexture(GL_TEXTURE_2D, mTextureID);
		glTexImage2D(GL_TEXTURE_2D
			, 0
			, mInternalChannel.platform
			, width, high
			, 0
			, mChannel.platform
			, mDataType.platform
			, NULL);

		if (mWrap.tiny == TextureWrap::Clamp_To_Border)
		{
			float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mWrap.platform);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mWrap.platform);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mFilter.platform);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mFilter.platform);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//----------------------------------------------
	//
	//	RenderBuffer2D
	//
	GLTextureRenderBuffer2D::GLTextureRenderBuffer2D()
	{
		glGenRenderbuffers(1, &mTextureID);
	}

	GLTextureRenderBuffer2D::~GLTextureRenderBuffer2D()
	{
		glDeleteRenderbuffers(1, &mTextureID);
	}

	void GLTextureRenderBuffer2D::create(const int& width, const int& high, const TextureChannel& internalChannel)
	{
		mInternalChannel = ContextMap::TextureChannelArray[(uint32_t)internalChannel];
		glBindRenderbuffer(GL_RENDERBUFFER, mTextureID);
		glRenderbufferStorage(GL_RENDERBUFFER, mInternalChannel.platform, width, high);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	//----------------------------------------------
	//
	//	Creator
	//
	Texture2D* GLTextureCreator::create2D()
	{
		return new GLTexture2D();
	}

	TextureCube* GLTextureCreator::createCube()
	{
		return new GLTextureCube();
	}

	TextureBuffer2D* GLTextureCreator::createBuffer2D()
	{
		return new GLTextureBuffer2D();
	}

	TextureRenderBuffer2D* GLTextureCreator::createRenderBuffer2D()
	{
		return new GLTextureRenderBuffer2D();
	}
}
