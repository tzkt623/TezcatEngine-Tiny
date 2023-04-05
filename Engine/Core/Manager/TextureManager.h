#pragma once
#include "../Manager/Manager.h"
#include "../Head/CppHead.h"
#include "../Head/Context.h"

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

	class TINY_API TextureManager : public Manager<TextureCreator>
	{
	public:
		TextureManager();
		virtual ~TextureManager();

		Texture* tryGetTexture(const std::string& filePath, const TextureType& textureType, const TextureFilter& filter = TextureFilter::Linear, const TextureWrap& wrap = TextureWrap::Repeat);

	private:
		std::unordered_map<std::string, Texture*> mTextureMap;
	};

	using TextureMgr = SG<TextureManager>;
}
