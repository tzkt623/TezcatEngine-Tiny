#pragma once
#include "GLHead.h"

namespace tezcat::Tiny::Core
{
	enum TexWrap
	{
		Tex_REPEAT = GL_REPEAT,
		Tex_MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
		Tex_CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
		Tex_CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
	};

	enum TexFilter
	{
		Tex_NEAREST = GL_NEAREST,
		Tex_LINEAR = GL_LINEAR
	};

	class Image;
	class Texture
	{
	public:
		Texture();
		~Texture();

		void createTexture(Image* image);
		void bind();

	private:
		TexWrap m_Wrap;
		TexFilter m_Filter;

		unsigned int m_TexID;
	};
}