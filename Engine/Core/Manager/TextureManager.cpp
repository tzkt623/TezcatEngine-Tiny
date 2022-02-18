#include "TextureManager.h"


namespace tezcat::Tiny::Core
{
	TextureManager::TextureManager()
		: m_Creator(nullptr)
	{

	}

	TextureManager::TextureManager(TextureCreator* creator)
		: m_Creator(creator)
	{

	}

	TextureManager::~TextureManager()
	{
		delete m_Creator;
	}

	void TextureManager::setCreator(TextureCreator* creator)
	{
		m_Creator = creator;
	}

	Texture* TextureManager::tryGetTexture(const char* filePath, const TextureType& textureType)
	{
		auto it = m_TextureMap.try_emplace(filePath, nullptr);
		if (it.second)
		{
			it.first->second = m_Creator->create(filePath, textureType);
		}

		return it.first->second;
	}
}
