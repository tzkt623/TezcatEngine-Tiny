#pragma once
#include "GLHead.h"

namespace tezcat::Tiny::Core
{
	class Image;
	class Shader;
	class Texture
	{
	public:
		enum Wrap : int
		{
			Tex_REPEAT = GL_REPEAT,
			Tex_MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
			Tex_CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
			Tex_CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
		};

		enum Filter : int
		{
			Tex_NEAREST = GL_NEAREST,
			Tex_LINEAR = GL_LINEAR
		};
	public:
		Texture();
		~Texture();

		void createTexture(Image* image);
		unsigned int getTexID() const { return m_TexID; }


	private:
		Wrap m_Wrap;
		Filter m_Filter;

		unsigned int m_TexID;
	};
}