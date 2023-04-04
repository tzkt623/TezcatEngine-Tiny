#include "TextureManager.h"


namespace tezcat::Tiny::Core
{
	TextureManager::TextureManager()
		: mCreator(nullptr)
	{

	}

	TextureManager::TextureManager(TextureCreator* creator)
		: mCreator(creator)
	{

	}

	TextureManager::~TextureManager()
	{
		delete mCreator;
	}

	void TextureManager::setCreator(TextureCreator* creator)
	{
		mCreator = creator;
	}

	Texture* TextureManager::tryGetTexture(const std::string& filePath, const TextureType& textureType, const TextureFilter& filter, const TextureWrap& wrap)
	{
		auto it = mTextureMap.try_emplace(filePath, nullptr);
		if (it.second)
		{
			it.first->second = mCreator->create(filePath, textureType, filter, wrap);
		}

		return it.first->second;
	}
}
