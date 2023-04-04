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
		virtual void createTexture(const std::string& filePath);

		uint32_t getTextureID() const { return mTextureID; }

		TexWrapWrapper& getWrap() { return mWrap; }
		void setWrap(const TextureWrap& val)
		{
			mWrap = ContextMap::TextureWrapArray[(int)val];
		}

		TexFilterWrapper& getFilter() { return mFilter; }
		void setFilter(const TextureFilter& val)
		{
			mFilter = ContextMap::TextureFilterArray[(int)val];
		}

	protected:
		uint32_t mTextureID;
		TexWrapWrapper mWrap;
		TexFilterWrapper mFilter;
	};

	class TINY_API Texture2D : public Texture
	{
	public:
		TextureType getTextureType() const final { return TextureType::Texture2D; }
	};

	class TINY_API Texture3D : public Texture
	{
	public:
		TextureType getTextureType() const final { return TextureType::Texture3D; }
	};

	class TINY_API TextureCube : public Texture
	{
	public:
		TextureType getTextureType() const final { return TextureType::TextureCube; }

	private:
		void createTexture(Image* image) final {}
	};
}
