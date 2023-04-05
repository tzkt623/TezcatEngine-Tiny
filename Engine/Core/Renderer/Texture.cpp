#include "Texture.h"
#include "../Data/Image.h"


namespace tezcat::Tiny::Core
{
	Texture::Texture()
		: Texture(TextureChannel::None, TextureWrap::Clamp_To_Edge, TextureFilter::Linear)
	{

	}

	Texture::Texture(const TextureChannel& internalChannel, const TextureWrap& wrap, const TextureFilter& filter)
		: mTextureID(0)
		, mWrap(ContextMap::TextureWrapArray[(uint32_t)wrap])
		, mFilter(ContextMap::TextureFilterArray[(uint32_t)filter])
		, mInternalChannel(ContextMap::TextureChannelArray[(uint32_t)internalChannel])
		, mChannel(ContextMap::TextureChannelArray[(uint32_t)TextureChannel::None])
		, mDataType(ContextMap::DataTypeArray[(uint32_t)DataType::UByte])
	{

	}

	Texture::~Texture()
	{

	}

	void Texture::createTexture(const std::string& filePath)
	{
		Image img;
		img.openFile(filePath);
		this->createTexture(&img);
	}


	TexChannelWrapper Texture::getTextureChannels(Image* image)
	{
		switch (image->getChannels())
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
}
