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
		: Texture2D(TextureWrap::Repeat
				  , TextureWrap::Repeat
				  , TextureAttachPosition::ColorComponent
				  , TextureChannel::None
				  , TextureChannel::None
				  , TextureFilter::Linear
				  , TextureFilter::Linear
				  , DataType::UByte)
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
		mIsHDR = image.isHDR();

		mWrapS = ContextMap::TextureWrapArray[(uint32_t)info.wrapS];
		mWrapT = ContextMap::TextureWrapArray[(uint32_t)info.wrapT];
		mMinFilter = ContextMap::TextureFilterArray[(uint32_t)info.minFilter];
		mMagFilter = ContextMap::TextureFilterArray[(uint32_t)info.magFilter];
		mInternalChannel = ContextMap::TextureChannelArray[(uint32_t)info.internalChannel];
		mChannel = ContextMap::TextureChannelArray[(uint32_t)info.channel];
		mDataType = ContextMap::DataTypeArray[(uint32_t)info.dataType];

		glBindTexture(GL_TEXTURE_2D, mTextureID);
		glTexImage2D(GL_TEXTURE_2D
				   , 0
				   , mInternalChannel.platform
				   , mWidth, mHeight
				   , 0
				   , mChannel.platform
				   , mDataType.platform
				   , image.getData());

		if (mWrapS.tiny == TextureWrap::Clamp_To_Border
			|| mWrapT.tiny == TextureWrap::Clamp_To_Border)
		{
			float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mWrapS.platform);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mWrapT.platform);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mMinFilter.platform);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mMagFilter.platform);

		this->genMipmap();

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

		if (mWrapS.tiny == TextureWrap::Clamp_To_Border
			|| mWrapT.tiny == TextureWrap::Clamp_To_Border)
		{
			float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mWrapS.platform);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mWrapT.platform);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mMinFilter.platform);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mMagFilter.platform);

		this->genMipmap();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLTexture2D::create(const int& width, const int& height, const TextureInfo& info)
	{
		mWidth = width;
		mHeight = height;

		mWrapS = ContextMap::TextureWrapArray[(uint32_t)info.wrapS];
		mWrapT = ContextMap::TextureWrapArray[(uint32_t)info.wrapT];
		mMinFilter = ContextMap::TextureFilterArray[(uint32_t)info.minFilter];
		mMagFilter = ContextMap::TextureFilterArray[(uint32_t)info.magFilter];
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

		if (mWrapS.tiny == TextureWrap::Clamp_To_Border
			|| mWrapT.tiny == TextureWrap::Clamp_To_Border)
		{
			float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mWrapS.platform);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mWrapT.platform);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mMinFilter.platform);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mMagFilter.platform);

		this->genMipmap();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLTexture2D::genMipmap()
	{
		if (mMinFilter.tiny == TextureFilter::Linear_Mipmap_Linear
			|| mMagFilter.tiny == TextureFilter::Linear_Mipmap_Linear)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 5);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}

	void GLTexture2D::bind()
	{
		glBindTexture(GL_TEXTURE_2D, mTextureID);
	}

	void GLTexture2D::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//-------------------------------------------
	//
	//	Cube
	//
	TINY_RTTI_CPP(GLTextureCube);
	GLTextureCube::GLTextureCube()
		: TextureCube(TextureWrap::Repeat
					, TextureWrap::Repeat
					, TextureWrap::Repeat
					, TextureAttachPosition::ColorComponent
					, TextureChannel::None
					, TextureChannel::None
					, TextureFilter::Linear
					, TextureFilter::Linear
					, DataType::UByte)
	{
		glGenTextures(1, &mTextureID);
	}

	GLTextureCube::~GLTextureCube()
	{
		glDeleteTextures(1, &mTextureID);
	}

	void GLTextureCube::create(const std::array<Image, 6>& images, const TextureInfo& info)
	{
		mMinFilter = ContextMap::TextureFilterArray[(uint32_t)info.minFilter];
		mMagFilter = ContextMap::TextureFilterArray[(uint32_t)info.magFilter];
		mWrapS = ContextMap::TextureWrapArray[(uint32_t)info.wrapS];
		mWrapT = ContextMap::TextureWrapArray[(uint32_t)info.wrapT];
		mWrapR = ContextMap::TextureWrapArray[(uint32_t)info.wrapR];
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

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, mWrapS.platform);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, mWrapT.platform);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, mWrapR.platform);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, mMinFilter.platform);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, mMagFilter.platform);

		this->genMipmap();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLTextureCube::create(const int& width, const int& hegiht, const TextureInfo& info)
	{
		mMinFilter = ContextMap::TextureFilterArray[(uint32_t)info.minFilter];
		mMagFilter = ContextMap::TextureFilterArray[(uint32_t)info.magFilter];
		mWrapS = ContextMap::TextureWrapArray[(uint32_t)info.wrapS];
		mWrapT = ContextMap::TextureWrapArray[(uint32_t)info.wrapT];
		mWrapR = ContextMap::TextureWrapArray[(uint32_t)info.wrapR];
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

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, mWrapS.platform);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, mWrapT.platform);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, mWrapR.platform);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, mMinFilter.platform);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, mMagFilter.platform);

		this->genMipmap();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLTextureCube::genMipmap()
	{
		if (mMinFilter.tiny == TextureFilter::Linear_Mipmap_Linear
			|| mMagFilter.tiny == TextureFilter::Linear_Mipmap_Linear)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 5);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}
	}

	void GLTextureCube::bind()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);
	}

	void GLTextureCube::unbind()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
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

	void GLTextureRender2D::bind()
	{
		glBindRenderbuffer(GL_RENDERBUFFER, mTextureID);
	}

	void GLTextureRender2D::setSize(const int& width, const int& height)
	{
		Texture2D::setSize(width, height);
		glRenderbufferStorage(GL_RENDERBUFFER, mInternalChannel.platform, width, height);
	}

	void GLTextureRender2D::unbind()
	{
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
