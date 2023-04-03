#pragma once
#include "../Head/CppHead.h"
#include "../Head/Context.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class Image;
	class TINY_API Texture
	{
	public:
		Texture();
		Texture(const TextureWrap& wrap, const TextureFilter& filter);
		virtual ~Texture();

		virtual TextureType getTextureType() const = 0;
		virtual void createTexture(Image* image) = 0;
		void createTexture(const char* filePath);

		inline uint32_t getTextureID() const { return mTextureID; }

		inline TexWrapWrapper& getWrap() { return m_Wrap; }
		void setWrap(const TextureWrap& val)
		{
			m_Wrap = ContextMap::TextureWrapArray[(int)val];
		}

		inline TexFilterWrapper& getFilter() { return m_Filter; }
		void setFilter(const TextureFilter& val)
		{
			m_Filter = ContextMap::TextureFilterArray[(int)val];
		}

	protected:
		uint32_t mTextureID;
		TexWrapWrapper m_Wrap;
		TexFilterWrapper m_Filter;
	};

	class TINY_API Texture2D : public Texture
	{
		TextureType getTextureType() const final { return TextureType::Texture2D; }
	};

	class TINY_API Texture3D : public Texture
	{
		TextureType getTextureType() const final { return TextureType::Texture3D; }
	};
}
