#include "Texture.h"
#include "../Data/Image.h"


namespace tezcat::Tiny
{
	uint32_t Texture::sUIDGiver = 0;
	std::deque<uint32_t> Texture::sFreeUIDs;

	TINY_RTTI_CPP(Texture)
	Texture::Texture()
		: Texture(TextureChannel::None, TextureWrap::Repeat, TextureFilter::Linear, false)
	{

	}

	Texture::Texture(const TextureChannel& internalChannel, const TextureWrap& wrap, const TextureFilter& filter, const bool& isManagered)
		: mUID(giveUID())
		, mTextureID(0)
		, mWrap(ContextMap::TextureWrapArray[(uint32_t)wrap])
		, mFilter(ContextMap::TextureFilterArray[(uint32_t)filter])
		, mInternalChannel(ContextMap::TextureChannelArray[(uint32_t)internalChannel])
		, mChannel(ContextMap::TextureChannelArray[(uint32_t)TextureChannel::None])
		, mDataType(ContextMap::DataTypeArray[(uint32_t)DataType::UByte])
		, mIsManagered(isManagered)
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
	TINY_RTTI_CPP(Texture2D)
	Texture2D::Texture2D()
		: mWidth(0)
		, mHeight(0)
	{

	}

	TINY_RTTI_CPP(Texture3D)
	TINY_RTTI_CPP(TextureCube)
	TINY_RTTI_CPP(TextureRenderBuffer2D)
	TINY_RTTI_CPP(TextureBuffer2D)

}
