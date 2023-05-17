#include "GLTexture.h"
#include "Core/Data/Image.h"
#include "Core/Head/RenderConfig.h"

using namespace tezcat::Tiny;
namespace tezcat::Tiny::GL
{
	//--------------------------------------------------------
	//
	//	Texture2D
	//
	TINY_RTTI_CPP(GLTexture2D);
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
		mWidth = image.getWidth();
		mHeight = image.getHeight();

		mWrap = ContextMap::TextureWrapArray[(uint32_t)info.wrap];
		mFilter = ContextMap::TextureFilterArray[(uint32_t)info.filter];
		mInternalChannel = ContextMap::TextureChannelArray[(uint32_t)info.internalChannel];
		mChannel = ContextMap::TextureChannelArray[(uint32_t)info.channel];
		mDataType = ContextMap::DataTypeArray[(uint32_t)info.dataType];

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

	void GLTexture2D::create(const int& width, const int& height
							, const TextureChannel& internalChannel
							, const TextureChannel& channel
							, const DataType& dataType)
	{
		mWidth = width;
		mHeight = height;

		mInternalChannel = ContextMap::TextureChannelArray[(uint32_t)internalChannel];
		mChannel = ContextMap::TextureChannelArray[(uint32_t)channel];
		mDataType = ContextMap::DataTypeArray[(uint32_t)dataType];

		glBindTexture(GL_TEXTURE_2D, mTextureID);
		glTexImage2D(GL_TEXTURE_2D
					, 0
					, mInternalChannel.platform
					, width, height
					, 0
					, mChannel.platform
					, mDataType.platform
					, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mWrap.platform);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mWrap.platform);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mFilter.platform);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mFilter.platform);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLTexture2D::create(const int& width, const int& height, const TextureInfo& info)
	{
		mWidth = width;
		mHeight = height;

		mWrap = ContextMap::TextureWrapArray[(uint32_t)info.wrap];
		mFilter = ContextMap::TextureFilterArray[(uint32_t)info.filter];
		mInternalChannel = ContextMap::TextureChannelArray[(uint32_t)info.internalChannel];
		mChannel = ContextMap::TextureChannelArray[(uint32_t)info.channel];
		mDataType = ContextMap::DataTypeArray[(uint32_t)info.dataType];

		glBindTexture(GL_TEXTURE_2D, mTextureID);
		glTexImage2D(GL_TEXTURE_2D
					, 0
					, mInternalChannel.platform
					, width, height
					, 0
					, mChannel.platform
					, mDataType.platform
					, nullptr);

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

	//-------------------------------------------
	//
	//	Cube
	//
	TINY_RTTI_CPP(GLTextureCube);
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
		mDataType = ContextMap::DataTypeArray[(uint32_t)info.dataType];

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

	void GLTextureCube::create(const int& width, const int& hegiht, const TextureInfo& info)
	{
		mFilter = ContextMap::TextureFilterArray[(uint32_t)info.filter];
		mWrap = ContextMap::TextureWrapArray[(uint32_t)info.wrap];
		mDataType = ContextMap::DataTypeArray[(uint32_t)info.dataType];
		mInternalChannel = ContextMap::TextureChannelArray[(uint32_t)info.internalChannel];
		mChannel = ContextMap::TextureChannelArray[(uint32_t)info.channel];

		glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);

		for (unsigned int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i
						, 0
						, mInternalChannel.platform
						, width, hegiht
						, 0
						, mChannel.platform
						, mDataType.platform
						, nullptr);
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
	//	RenderBuffer2D
	//
	TINY_RTTI_CPP(GLTextureRender2D);
	GLTextureRender2D::GLTextureRender2D()
	{
		glGenRenderbuffers(1, &mTextureID);
	}

	GLTextureRender2D::~GLTextureRender2D()
	{
		glDeleteRenderbuffers(1, &mTextureID);
	}

	void GLTextureRender2D::create(const int& width, const int& height
		, const TextureChannel& internalChannel)
	{
		mWidth = width;
		mHeight = height;

		mInternalChannel = ContextMap::TextureChannelArray[(uint32_t)internalChannel];
		glBindRenderbuffer(GL_RENDERBUFFER, mTextureID);
		glRenderbufferStorage(GL_RENDERBUFFER, mInternalChannel.platform, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	//----------------------------------------------
	//
	//	Creator
	//
	Texture2D* GLTextureCreator::create2D()
	{
		return GLTexture2D::create();
	}

	TextureCube* GLTextureCreator::createCube()
	{
		return GLTextureCube::create();
	}

	TextureRender2D* GLTextureCreator::createRender2D()
	{
		return GLTextureRender2D::create();
	}
}
