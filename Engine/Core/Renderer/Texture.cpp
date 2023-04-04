#include "Texture.h"
#include "../Data/Image.h"


namespace tezcat::Tiny::Core
{
	Texture::Texture()
		: Texture(TextureWrap::Tex_REPEAT, TextureFilter::Tex_LINEAR)
	{

	}

	Texture::Texture(const TextureWrap& wrap, const TextureFilter& filter)
		: mTextureID(0)
		, mWrap(ContextMap::TextureWrapArray[(int)wrap])
		, mFilter(ContextMap::TextureFilterArray[(int)filter])
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
}
