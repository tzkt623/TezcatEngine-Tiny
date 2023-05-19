#include "Texture.h"
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
	//
	//
	TINY_RTTI_CPP(Texture2D);
	Texture2D::Texture2D(const TextureWrap& wrapS
					   , const TextureWrap& wrapT
					   , const TextureAttachPosition& attachPosition
					   , const TextureChannel& internalChannel
					   , const TextureChannel& channel
					   , const TextureFilter& minFilter
					   , const TextureFilter& magFilter
					   , const DataType& dataType)
		: Texture(attachPosition, internalChannel, channel, minFilter, magFilter, dataType)
		, mWidth(0)
		, mHeight(0)
		, mWrapS(ContextMap::TextureWrapArray[(uint32_t)wrapS])
		, mWrapT(ContextMap::TextureWrapArray[(uint32_t)wrapT])
		, mIsHDR(false)
	{

	}

	TINY_RTTI_CPP(Texture3D);
	TINY_RTTI_CPP(TextureCube);
	TextureCube::TextureCube(const TextureWrap& wrapS
				   , const TextureWrap& wrapT
				   , const TextureWrap& wrapR
				   , const TextureAttachPosition& attachPosition
				   , const TextureChannel& internalChannel
				   , const TextureChannel& channel
				   , const TextureFilter& minFilter
				   , const TextureFilter& magFilter
				   , const DataType& dataType)
		: Texture(attachPosition, internalChannel, channel, minFilter, magFilter, dataType)
		, mWrapS(ContextMap::TextureWrapArray[(uint32_t)wrapS])
		, mWrapT(ContextMap::TextureWrapArray[(uint32_t)wrapT])
		, mWrapR(ContextMap::TextureWrapArray[(uint32_t)wrapR])
	{

	}
	TINY_RTTI_CPP(TextureRender2D);
	TextureRender2D::TextureRender2D()
		: Texture2D(TextureWrap::Repeat
				  , TextureWrap::Repeat
				  , TextureAttachPosition::ColorComponent
				  , TextureChannel::None
				  , TextureChannel::None
				  , TextureFilter::Linear
				  , TextureFilter::Linear
				  , DataType::UByte)
	{

	}

}
