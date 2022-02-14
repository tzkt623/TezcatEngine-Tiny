#pragma once
#include "../Head/GLHead.h"
#include "../Head/CppHead.h"

namespace tezcat::Tiny::Core
{
	class Image;
	class Texture
	{
	public:

	public:
		Texture();
		~Texture();

		void createTexture(Image* image);
		void createTexture(const std::string& filePath);
		unsigned int getTexID() const { return m_TexID; }


	private:
		TexWrap m_Wrap;
		TexFilter m_Filter;

		unsigned int m_TexID;
	};
}