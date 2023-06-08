#include "Texture.h"
#include "BaseGraphics.h"
#include "../Manager/TextureManager.h"
#include "../Data/Image.h"


namespace tezcat::Tiny
{
	uint32_t Texture::sUIDGiver = 0;
	std::deque<uint32_t> Texture::sFreeUIDs;

	TINY_RTTI_CPP(Texture);
	Texture::Texture(const TextureAttachPosition& attachPosition
				   , const TextureChannel& internalChannel
				   , const TextureChannel& channel
				   , const TextureFilter& minFilter
				   , const TextureFilter& magFilter
				   , const DataType& dataType)
		: mUID(giveUID())
		, mTextureID(0)
		, mAttachPosition(attachPosition)
		, mMinFilter(ContextMap::TextureFilterArray[(uint32_t)minFilter])
		, mMagFilter(ContextMap::TextureFilterArray[(uint32_t)magFilter])
		, mInternalChannel(ContextMap::TextureChannelArray[(uint32_t)internalChannel])
		, mChannel(ContextMap::TextureChannelArray[(uint32_t)channel])
		, mDataType(ContextMap::DataTypeArray[(uint32_t)dataType])
		, mName()
	{

	}

	Texture::~Texture()
	{
		sFreeUIDs.push_back(mUID);
	}

	TexChannelWrapper Texture::getTextureChannels(const Image& image)
	{
		switch (image.getChannels())
		{
		case 1: return ContextMap::TextureChannelArray[(uint32_t)TextureChannel::R];
		case 2: return ContextMap::TextureChannelArray[(uint32_t)TextureChannel::RG];
		case 3: return ContextMap::TextureChannelArray[(uint32_t)TextureChannel::RGB];
		case 4: return ContextMap::TextureChannelArray[(uint32_t)TextureChannel::RGBA];
		default:
			break;
		}

		return ContextMap::TextureChannelArray[(uint32_t)TextureChannel::None];
	}

	uint32_t Texture::giveUID()
	{
		if (sFreeUIDs.empty())
		{
			return sUIDGiver++;
		}

		auto uid = sFreeUIDs.front();
		sFreeUIDs.pop_front();
		return uid;
	}

	//------------------------------------------------------------
	//
	//	Texture2D
	//
	TINY_RTTI_CPP(Texture2D);
	Texture2D::Texture2D()
		: Texture(TextureAttachPosition::ColorComponent
			, TextureChannel::RGBA
			, TextureChannel::RGBA
			, TextureFilter::Linear
			, TextureFilter::Linear
			, DataType::UByte)
		, mWidth(0)
		, mHeight(0)
		, mIsHDR(false)
		, mData(nullptr)
	{
	}

	Texture2D::Texture2D(const std::string& name)
		: Texture2D()
	{
		mName = name;
	}

	Texture2D::~Texture2D()
	{
		Graphics::getInstance()->cmdDeleteTexture2D(mTextureID);
	}

	void Texture2D::setData(const Image* image, const TextureInfo& info)
	{
		mWidth = image->getWidth();
		mHeight = image->getHeight();
		mIsHDR = image->isHDR();

		uint64_t size = image->getDataSize();
		auto temp = realloc(mData, size);
		if (temp)
		{
			mData = temp;
			memcpy_s(mData, size, image->getData(), size);
		}

		mAttachPosition = info.attachPosition;
		mWrapS = ContextMap::TextureWrapArray[(uint32_t)info.wrapS];
		mWrapT = ContextMap::TextureWrapArray[(uint32_t)info.wrapT];
		mMinFilter = ContextMap::TextureFilterArray[(uint32_t)info.minFilter];
		mMagFilter = ContextMap::TextureFilterArray[(uint32_t)info.magFilter];
		mInternalChannel = mIsHDR ? ContextMap::TextureChannelArray[(uint32_t)TextureChannel::RGB16f] : getTextureChannels(*image);
		mChannel = getTextureChannels(*image);
		mDataType = ContextMap::DataTypeArray[(uint32_t)(image->isHDR() ? DataType::Float32 : DataType::UByte)];
	}

	void Texture2D::setData(const Image* image)
	{
		mWidth = image->getWidth();
		mHeight = image->getHeight();
		mIsHDR = image->isHDR();

		uint64_t size = image->getDataSize();
		auto temp = realloc(mData, size);
		if (temp)
		{
			mData = temp;
			memcpy_s(mData, size, image->getData(), size);
		}

		mAttachPosition = TextureAttachPosition::ColorComponent;
		mWrapS = ContextMap::TextureWrapArray[(uint32_t)TextureWrap::Clamp_To_Edge];
		mWrapT = ContextMap::TextureWrapArray[(uint32_t)TextureWrap::Clamp_To_Edge];
		mMinFilter = ContextMap::TextureFilterArray[(uint32_t)TextureFilter::Linear];
		mMagFilter = ContextMap::TextureFilterArray[(uint32_t)TextureFilter::Linear];
		mInternalChannel = mIsHDR ? ContextMap::TextureChannelArray[(uint32_t)TextureChannel::RGB16f] : getTextureChannels(*image);
		mChannel = getTextureChannels(*image);
		mDataType = ContextMap::DataTypeArray[(uint32_t)(image->isHDR() ? DataType::Float32 : DataType::UByte)];
	}


	void Texture2D::setData(const int& width, const int& height, const TextureInfo& info)
	{
		mWidth = width;
		mHeight = height;

		mAttachPosition = info.attachPosition;
		mWrapS = ContextMap::TextureWrapArray[(uint32_t)info.wrapS];
		mWrapT = ContextMap::TextureWrapArray[(uint32_t)info.wrapT];
		mMinFilter = ContextMap::TextureFilterArray[(uint32_t)info.minFilter];
		mMagFilter = ContextMap::TextureFilterArray[(uint32_t)info.magFilter];
		mInternalChannel = ContextMap::TextureChannelArray[(uint32_t)info.internalChannel];
		mChannel = ContextMap::TextureChannelArray[(uint32_t)info.channel];
		mDataType = ContextMap::DataTypeArray[(uint32_t)info.dataType];
	}

	void Texture2D::updateData(const Image* image)
	{
		mWidth = image->getWidth();
		mHeight = image->getHeight();
		mIsHDR = image->isHDR();

		mInternalChannel = getTextureChannels(*image);
		mChannel = getTextureChannels(*image);
		mDataType = ContextMap::DataTypeArray[(uint32_t)(image->isHDR() ? DataType::Float32 : DataType::UByte)];

		uint64_t size = image->getDataSize();
		auto temp = realloc(mData, size);
		if (temp)
		{
			mData = temp;
			memcpy_s(mData, size, image->getData(), size);
		}
	}

	void Texture2D::generate()
	{
		Graphics::getInstance()->cmdCreateTexture2D(this);
	}

	void Texture2D::update()
	{
		Graphics::getInstance()->cmdUpdateTexture2D(this);
	}

	void Texture2D::apply(uint32_t id)
	{
		Texture::apply(id);
		free(mData);
		mData = nullptr;
	}

	//--------------------------------------------------------
	//
	//	Texture3D
	//
	TINY_RTTI_CPP(Texture3D);


	Texture3D::~Texture3D()
	{

	}
	//--------------------------------------------------------
	//
	//	TextureCubes
	//
	TINY_RTTI_CPP(TextureCube);
	TextureCube::TextureCube()
		: Texture(TextureAttachPosition::ColorComponent
			, TextureChannel::RGBA
			, TextureChannel::RGBA
			, TextureFilter::Linear
			, TextureFilter::Linear
			, DataType::UByte)
	{
		mDatas[0] = nullptr;
		mDatas[1] = nullptr;
		mDatas[2] = nullptr;
		mDatas[3] = nullptr;
		mDatas[4] = nullptr;
		mDatas[5] = nullptr;
	}

	TextureCube::TextureCube(const std::string& name)
		: Texture(TextureAttachPosition::ColorComponent
			, TextureChannel::RGBA
			, TextureChannel::RGBA
			, TextureFilter::Linear
			, TextureFilter::Linear
			, DataType::UByte)
	{
		mDatas[0] = nullptr;
		mDatas[1] = nullptr;
		mDatas[2] = nullptr;
		mDatas[3] = nullptr;
		mDatas[4] = nullptr;
		mDatas[5] = nullptr;
		mName = name;
	}

	TextureCube::~TextureCube()
	{
		Graphics::getInstance()->cmdDeleteTextureCube(mTextureID);
	}

	void TextureCube::setData(const int& width, const int& height, const TextureInfo& info)
	{
		mWidth = width;
		mHeight = height;

		mAttachPosition = info.attachPosition;
		mMinFilter = ContextMap::TextureFilterArray[(uint32_t)info.minFilter];
		mMagFilter = ContextMap::TextureFilterArray[(uint32_t)info.magFilter];
		mInternalChannel = ContextMap::TextureChannelArray[(uint32_t)info.internalChannel];
		mChannel = ContextMap::TextureChannelArray[(uint32_t)info.channel];
		mDataType = ContextMap::DataTypeArray[(uint32_t)info.dataType];

		mWrapS = ContextMap::TextureWrapArray[(uint32_t)info.wrapS];
		mWrapT = ContextMap::TextureWrapArray[(uint32_t)info.wrapT];
		mWrapR = ContextMap::TextureWrapArray[(uint32_t)info.wrapR];
	}

	void TextureCube::setData(const std::array<Image*, 6>& images)
	{
		mWidth = images[0]->getWidth();
		mHeight = images[0]->getHeight();

		mAttachPosition = TextureAttachPosition::ColorComponent;
		mMinFilter = ContextMap::TextureFilterArray[(uint32_t)TextureFilter::Linear];
		mMagFilter = ContextMap::TextureFilterArray[(uint32_t)TextureFilter::Linear];
		mInternalChannel = getTextureChannels(*images[0]);
		mChannel = getTextureChannels(*images[0]);
		mDataType = ContextMap::DataTypeArray[(uint32_t)DataType::UByte];

		mWrapS = ContextMap::TextureWrapArray[(uint32_t)TextureWrap::Clamp_To_Edge];
		mWrapT = ContextMap::TextureWrapArray[(uint32_t)TextureWrap::Clamp_To_Edge];
		mWrapR = ContextMap::TextureWrapArray[(uint32_t)TextureWrap::Clamp_To_Edge];

		for (size_t i = 0; i < images.size(); i++)
		{
			auto size = images[i]->getDataSize();
			auto temp = realloc(mDatas[i], size);
			if (temp)
			{
				mDatas[i] = temp;
				memcpy_s(mDatas[i], size, images[i]->getData(), size);
			}
		}
	}


	void TextureCube::setData(const std::array<Image*, 6>& images, const TextureInfo& info)
	{
		mAttachPosition = info.attachPosition;
		mMinFilter = ContextMap::TextureFilterArray[(uint32_t)info.minFilter];
		mMagFilter = ContextMap::TextureFilterArray[(uint32_t)info.magFilter];
		mInternalChannel = ContextMap::TextureChannelArray[(uint32_t)info.internalChannel];
		mChannel = ContextMap::TextureChannelArray[(uint32_t)info.channel];
		mDataType = ContextMap::DataTypeArray[(uint32_t)info.dataType];

		mWrapS = ContextMap::TextureWrapArray[(uint32_t)info.wrapS];
		mWrapT = ContextMap::TextureWrapArray[(uint32_t)info.wrapT];
		mWrapR = ContextMap::TextureWrapArray[(uint32_t)info.wrapR];

		for (size_t i = 0; i < images.size(); i++)
		{
			mWidth = images[i]->getWidth();
			mHeight = images[i]->getHeight();

			auto size = images[i]->getDataSize();
			auto temp = realloc(mDatas[i], size);
			if (temp)
			{
				mDatas[i] = temp;
				memcpy_s(mDatas[i], size, images[i]->getData(), size);
			}
		}
	}

	void TextureCube::generate()
	{
		Graphics::getInstance()->cmdCreateTextureCube(this);
	}

	void TextureCube::apply(uint32_t id)
	{
		Texture::apply(id);
		for (auto& d : mDatas)
		{
			free(d);
			d = nullptr;
		}
	}

	//--------------------------------------------------------
	//
	//	TextureRender2D
	//
	TINY_RTTI_CPP(TextureRender2D);
	TextureRender2D::TextureRender2D()
		: Texture(TextureAttachPosition::ColorComponent
			, TextureChannel::RGBA
			, TextureChannel::RGBA
			, TextureFilter::Linear
			, TextureFilter::Linear
			, DataType::UByte)
		, mWidth(0)
		, mHeight(0)
	{

	}

	TextureRender2D::TextureRender2D(const std::string& name)
		: Texture(TextureAttachPosition::ColorComponent
			, TextureChannel::RGBA
			, TextureChannel::RGBA
			, TextureFilter::Linear
			, TextureFilter::Linear
			, DataType::UByte)
		, mWidth(0)
		, mHeight(0)
	{
		mName = name;
	}

	TextureRender2D::~TextureRender2D()
	{
		Graphics::getInstance()->cmdDeleteRender2D(mTextureID);
	}

	void TextureRender2D::setData(const int& width, const int& height, const TextureInfo& info)
	{
		mWidth = width;
		mHeight = height;

		mAttachPosition = info.attachPosition;
		mWrapS = ContextMap::TextureWrapArray[(uint32_t)info.wrapS];
		mWrapT = ContextMap::TextureWrapArray[(uint32_t)info.wrapT];
		mMinFilter = ContextMap::TextureFilterArray[(uint32_t)info.minFilter];
		mMagFilter = ContextMap::TextureFilterArray[(uint32_t)info.magFilter];
		mInternalChannel = ContextMap::TextureChannelArray[(uint32_t)info.internalChannel];
		mChannel = ContextMap::TextureChannelArray[(uint32_t)info.channel];
		mDataType = ContextMap::DataTypeArray[(uint32_t)info.dataType];
	}

	void TextureRender2D::generate()
	{
		Graphics::getInstance()->cmdCreateRender2D(this);
	}

}
