#include "Texture.h"
#include "BaseGraphics.h"
#include "../Manager/TextureManager.h"
#include "../Data/Image.h"


namespace tezcat::Tiny
{
	uint32 Texture::sUIDGiver = 0;
	std::deque<uint32> Texture::sFreeUIDs;

	TINY_OBJECT_CPP(Texture, TinyObject)
	Texture::Texture(const TextureFormat& internalFormat
					   , const TextureFormat& format
					   , const TextureFilter& minFilter
					   , const TextureFilter& magFilter
					   , const DataMemFormat& dataType)
		: mUID(giveUID())
		, mTextureID(0)
		, mAttachPosition(TextureAttachPosition::None)
		, mMinFilter(ContextMap::TextureFilterArray[(uint32)minFilter])
		, mMagFilter(ContextMap::TextureFilterArray[(uint32)magFilter])
		, mInternalFormat(ContextMap::TextureInternalFormatArray[(uint32)internalFormat])
		, mFormat(ContextMap::TextureFormatArray[(uint32)format])
		, mDataMemFormat(ContextMap::DataMemFormatArray[(uint32)dataType])
		, mName()
	{

	}

	Texture::~Texture()
	{
		sFreeUIDs.push_back(mUID);
	}

	TexFormatWrapper Texture::getTextureFormat(const Image& image)
	{
		switch (image.getChannels())
		{
		case 1: return ContextMap::TextureFormatArray[(uint32)TextureFormat::R];
		case 2: return ContextMap::TextureFormatArray[(uint32)TextureFormat::RG];
		case 3: return ContextMap::TextureFormatArray[(uint32)TextureFormat::RGB];
		case 4: return ContextMap::TextureFormatArray[(uint32)TextureFormat::RGBA];
		default:
			break;
		}

		return ContextMap::TextureFormatArray[(uint32)TextureFormat::None];
	}

	uint32 Texture::giveUID()
	{
		if (sFreeUIDs.empty())
		{
			return sUIDGiver++;
		}

		auto uid = sFreeUIDs.front();
		sFreeUIDs.pop_front();
		return uid;
	}

	TexInternalFormatWrapper Texture::getTextureInternalFormat(const Image& image)
	{
		switch (image.getChannels())
		{
		case 1: return image.isHDR() ? ContextMap::TextureInternalFormatArray[(uint32)TextureInternalFormat::R16F] : ContextMap::TextureInternalFormatArray[(uint32)TextureInternalFormat::R];
		case 2: return image.isHDR() ? ContextMap::TextureInternalFormatArray[(uint32)TextureInternalFormat::RG16F] : ContextMap::TextureInternalFormatArray[(uint32)TextureInternalFormat::RG];
		case 3: return image.isHDR() ? ContextMap::TextureInternalFormatArray[(uint32)TextureInternalFormat::RGB16F] : ContextMap::TextureInternalFormatArray[(uint32)TextureInternalFormat::RGB];
		case 4: return image.isHDR() ? ContextMap::TextureInternalFormatArray[(uint32)TextureInternalFormat::RGBA16F] : ContextMap::TextureInternalFormatArray[(uint32)TextureInternalFormat::RGBA];
		default:
			break;
		}

		return ContextMap::TextureInternalFormatArray[(uint32)TextureInternalFormat::None];
	}

	//------------------------------------------------------------
	//
	//	Texture2D
	//
	TINY_OBJECT_CPP(Texture2D, Texture)
	Texture2D::Texture2D()
		: Base(TextureFormat::RGBA
			, TextureFormat::RGBA
			, TextureFilter::Linear
			, TextureFilter::Linear
			, DataMemFormat::UByte)
		, mWidth(0)
		, mHeight(0)
		, mIsHDR(false)
		, mData(nullptr)
	{

	}

	Texture2D::Texture2D(std::string name)
		: Texture2D()
	{
		mName.assign(std::move(name));
	}

	Texture2D::~Texture2D()
	{
		Graphics::getInstance()->cmdDeleteTexture2D(mTextureID);
	}

	void Texture2D::setImage(const Image* image)
	{
		mWidth = image->getWidth();
		mHeight = image->getHeight();
		mIsHDR = image->isHDR();

		uint64 size = image->getDataSize();
		auto temp = realloc(mData, size);
		if (temp)
		{
			mData = temp;
			memcpy_s(mData, size, image->getData(), size);
		}

		mAttachPosition = TextureAttachPosition::ColorComponent;
		mWrapS = ContextMap::TextureWrapArray[(uint32)TextureWrap::Clamp_To_Edge];
		mWrapT = ContextMap::TextureWrapArray[(uint32)TextureWrap::Clamp_To_Edge];
		mMinFilter = ContextMap::TextureFilterArray[(uint32)TextureFilter::Linear];
		mMagFilter = ContextMap::TextureFilterArray[(uint32)TextureFilter::Linear];
		mInternalFormat = getTextureInternalFormat(*image);
		mFormat = getTextureFormat(*image);
		mDataMemFormat = ContextMap::DataMemFormatArray[(uint32)(image->isHDR() ? DataMemFormat::Float : DataMemFormat::UByte)];
	}

	void Texture2D::setConfig(const uint32& width, const uint32& height
		, const TextureInternalFormat& internalFormat, const TextureFormat& format
		, const DataMemFormat& dataType
		, const TextureFilter& min /*= TextureFilter::Linear */
		, const TextureFilter& mag /*= TextureFilter::Linear */
		, const TextureWrap& wrapS /*= TextureWrap::Clamp_To_Edge */
		, const TextureWrap& wrapT /*= TextureWrap::Clamp_To_Edge*/)
	{
		mWidth = width;
		mHeight = height;

		mInternalFormat = ContextMap::TextureInternalFormatArray[(uint32)internalFormat];
		mFormat = ContextMap::TextureFormatArray[(uint32)format];
		mDataMemFormat = ContextMap::DataMemFormatArray[(uint32)dataType];

		mWrapS = ContextMap::TextureWrapArray[(uint32)wrapS];
		mWrapT = ContextMap::TextureWrapArray[(uint32)wrapT];
		mMinFilter = ContextMap::TextureFilterArray[(uint32)min];
		mMagFilter = ContextMap::TextureFilterArray[(uint32)mag];
	}

	void Texture2D::updateData(const Image* image)
	{
		mWidth = image->getWidth();
		mHeight = image->getHeight();
		mIsHDR = image->isHDR();

		mInternalFormat = getTextureInternalFormat(*image);
		mFormat = getTextureFormat(*image);
		mDataMemFormat = ContextMap::DataMemFormatArray[(uint32)(image->isHDR() ? DataMemFormat::Float : DataMemFormat::UByte)];

		free(mData);

		uint64 size = image->getDataSize();
		auto temp = realloc(mData, size);
		if (temp)
		{
			mData = temp;
			memcpy_s(mData, size, image->getData(), size);
		}
	}

	void Texture2D::generate()
	{
		if (mTextureID != 0)
		{
			Graphics::getInstance()->cmdDeleteTexture2D(mTextureID);
		}

		Graphics::getInstance()->cmdCreateTexture2D(this);
	}

	void Texture2D::update()
	{
		Graphics::getInstance()->cmdUpdateTexture2D(this);
	}

	void Texture2D::apply(uint32 id)
	{
		Texture::apply(id);
		free(mData);
		mData = nullptr;
	}

	//--------------------------------------------------------
	//
	//	Texture3D
	//
	TINY_OBJECT_CPP(Texture3D, Texture)
	Texture3D::~Texture3D()
	{

	}

	void Texture3D::setConfig(const uint32& width, const uint32& hegiht, const uint32& length
		, const TextureInternalFormat& internalFormat, const TextureFormat& format
		, const DataMemFormat& dataType
		, const TextureFilter& min /*= TextureFilter::Linear */
		, const TextureFilter& mag /*= TextureFilter::Linear */
		, const TextureWrap& wrapS /*= TextureWrap::Clamp_To_Edge */
		, const TextureWrap& wrapT /*= TextureWrap::Clamp_To_Edge */
		, const TextureWrap& wrapR /*= TextureWrap::Clamp_To_Edge*/)
	{
		mWidth = width;
		mHeight = hegiht;
		mLength = length;

		mInternalFormat = ContextMap::TextureInternalFormatArray[(uint32)internalFormat];
		mFormat = ContextMap::TextureFormatArray[(uint32)format];
		mDataMemFormat = ContextMap::DataMemFormatArray[(uint32)dataType];

		mMinFilter = ContextMap::TextureFilterArray[(uint32)min];
		mMagFilter = ContextMap::TextureFilterArray[(uint32)mag];

		mWrapS = ContextMap::TextureWrapArray[(uint32)wrapS];
		mWrapT = ContextMap::TextureWrapArray[(uint32)wrapT];
		mWrapR = ContextMap::TextureWrapArray[(uint32)wrapR];
	}

	//--------------------------------------------------------
	//
	//	TextureCubes
	//
	TINY_OBJECT_CPP(TextureCube, Texture)
	TextureCube::TextureCube()
		: Base(TextureFormat::RGBA
			, TextureFormat::RGBA
			, TextureFilter::Linear
			, TextureFilter::Linear
			, DataMemFormat::UByte)
		, mSize(0)
	{
		mDatas[0] = nullptr;
		mDatas[1] = nullptr;
		mDatas[2] = nullptr;
		mDatas[3] = nullptr;
		mDatas[4] = nullptr;
		mDatas[5] = nullptr;
	}

	TextureCube::TextureCube(std::string name)
		: TextureCube()
	{
		mDatas[0] = nullptr;
		mDatas[1] = nullptr;
		mDatas[2] = nullptr;
		mDatas[3] = nullptr;
		mDatas[4] = nullptr;
		mDatas[5] = nullptr;
		mName.assign(std::move(name));
	}

	TextureCube::~TextureCube()
	{
		Graphics::getInstance()->cmdDeleteTextureCube(mTextureID);
	}

	void TextureCube::setImage(const std::array<Image*, 6>& images)
	{
		TINY_ASSERT(images[0]->getWidth() == images[0]->getHeight());

		mSize = images[0]->getWidth();

		mAttachPosition = TextureAttachPosition::ColorComponent;
		mMinFilter = ContextMap::TextureFilterArray[(uint32)TextureFilter::Linear];
		mMagFilter = ContextMap::TextureFilterArray[(uint32)TextureFilter::Linear];
		mInternalFormat = getTextureInternalFormat(*images[0]);
		mFormat = getTextureFormat(*images[0]);
		mDataMemFormat = ContextMap::DataMemFormatArray[(uint32)(images[0]->isHDR() ? DataMemFormat::Float : DataMemFormat::UByte)];

		mWrapS = ContextMap::TextureWrapArray[(uint32)TextureWrap::Clamp_To_Edge];
		mWrapT = ContextMap::TextureWrapArray[(uint32)TextureWrap::Clamp_To_Edge];
		mWrapR = ContextMap::TextureWrapArray[(uint32)TextureWrap::Clamp_To_Edge];

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

	void TextureCube::setConfig(const uint32& size
		, const TextureInternalFormat& internalFormat, const TextureFormat& format
		, const DataMemFormat& dataType
		, const TextureFilter& min, const TextureFilter& mag
		, const TextureWrap& wrapS, const TextureWrap& wrapT, const TextureWrap& wrapR)
	{
		mSize = size;

		mInternalFormat = ContextMap::TextureInternalFormatArray[(uint32)internalFormat];
		mFormat = ContextMap::TextureFormatArray[(uint32)format];
		mDataMemFormat = ContextMap::DataMemFormatArray[(uint32)dataType];

		mMinFilter = ContextMap::TextureFilterArray[(uint32)min];
		mMagFilter = ContextMap::TextureFilterArray[(uint32)mag];

		mWrapS = ContextMap::TextureWrapArray[(uint32)wrapS];
		mWrapT = ContextMap::TextureWrapArray[(uint32)wrapT];
		mWrapR = ContextMap::TextureWrapArray[(uint32)wrapR];
	}

	void TextureCube::generate()
	{
		Graphics::getInstance()->cmdCreateTextureCube(this);
	}

	void TextureCube::apply(uint32 id)
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
	TINY_OBJECT_CPP(TextureRender2D, Texture)
	TextureRender2D::TextureRender2D()
		: Base(TextureFormat::RGBA
			, TextureFormat::RGBA
			, TextureFilter::Linear
			, TextureFilter::Linear
			, DataMemFormat::UByte)
		, mWidth(0)
		, mHeight(0)
	{

	}

	TextureRender2D::TextureRender2D(std::string name)
		: Base(TextureFormat::RGBA
			, TextureFormat::RGBA
			, TextureFilter::Linear
			, TextureFilter::Linear
			, DataMemFormat::UByte)
		, mWidth(0)
		, mHeight(0)
	{
		mName.assign(name);
	}

	TextureRender2D::~TextureRender2D()
	{
		Graphics::getInstance()->cmdDeleteRender2D(mTextureID);
	}

	void TextureRender2D::setConfig(const uint32& width, const uint32& height, const TextureInternalFormat& internalFormat)
	{
		mWidth = width;
		mHeight = height;
		mInternalFormat = ContextMap::TextureInternalFormatArray[(uint32)internalFormat];
	}

	void TextureRender2D::generate()
	{
		Graphics::getInstance()->cmdCreateRender2D(this);
	}
}
