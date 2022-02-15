#pragma once
#include "../Head/CppHead.h"
#include "../Head/ContextMap.h"

namespace tezcat::Tiny::Core
{
	class Image;
	class Texture
	{
	public:

	public:
		Texture();
		Texture(TextureWrap wrap, TextureFilter filter);
		virtual ~Texture();

		virtual void createTexture(Image* image) = 0;
		virtual void createTexture(const std::string& filePath) = 0;
		virtual TextureType getTextureType() const = 0;

		inline uint32_t getTextureID() const { return m_TextureID; }
		inline TextureWrap getWrap() const { return m_Wrap; }
		inline void setWrap(TextureWrap val) { m_Wrap = val; }
		inline TextureFilter getFilter() const { return m_Filter; }
		inline void setFilter(TextureFilter val) { m_Filter = val; }

	protected:
		uint32_t m_TextureID;
		TextureWrap m_Wrap;
		TextureFilter m_Filter;

	};
}