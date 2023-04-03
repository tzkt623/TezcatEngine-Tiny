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
		, m_Wrap(ContextMap::TextureWrapArray[(int)wrap])
		, m_Filter(ContextMap::TextureFilterArray[(int)filter])
	{
	}

	Texture::~Texture()
	{

	}

	void Texture::createTexture(Image* image)
	{

	}

	void Texture::createTexture(const char* filePath)
	{
		Image img;
		img.openFile(filePath);
		this->createTexture(&img);
	}
}
