#include "Texture.h"


namespace tezcat::Tiny::Core
{
	Texture::Texture()
		: Texture(TextureWrap::Tex_REPEAT, TextureFilter::Tex_LINEAR)
	{

	}

	Texture::Texture(TextureWrap wrap, TextureFilter filter)
		: m_TextureID(0)
		, m_Wrap(wrap)
		, m_Filter(filter)
	{
	}

	Texture::~Texture()
	{

	}
}
