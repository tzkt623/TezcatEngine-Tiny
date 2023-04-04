#pragma once

#include "../Head/CppHead.h"
#include "../Head/Context.h"
#include "../Head/ConfigHead.h"

#include "Utility/Utility.h"

namespace tezcat::Tiny::Core
{
	class Texture;
	class TINY_API TextureCreator
	{
	public:
		TextureCreator() {}
		virtual ~TextureCreator() {}

		virtual Texture* create(const std::string& filePath, const TextureType& textureType, const TextureFilter& filter, const TextureWrap& wrap) = 0;
	};

	class TINY_API TextureManager
	{
	public:
		TextureManager();
		TextureManager(TextureCreator* creator);
		~TextureManager();

		void setCreator(TextureCreator* creator);

		Texture* tryGetTexture(const std::string& filePath, const TextureType& textureType, const TextureFilter& filter = TextureFilter::Tex_LINEAR, const TextureWrap& wrap = TextureWrap::Tex_REPEAT);

	private:
		TextureCreator* mCreator;
		std::unordered_map<std::string, Texture*> mTextureMap;
	};

	using TextureMgr = SG<TextureManager>;
}
