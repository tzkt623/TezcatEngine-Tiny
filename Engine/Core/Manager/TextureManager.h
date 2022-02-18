#pragma once

#include "../Head/CppHead.h"
#include "../Head/Context.h"
#include "Utility/Tools.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class Texture;
	class TINY_API TextureCreator
	{
	public:
		TextureCreator() {}
		virtual ~TextureCreator() {}

		virtual Texture* create(const char* filePath, const TextureType& textureType) = 0;
	};

	class TINY_API TextureManager
	{
	public:
		TextureManager();
		TextureManager(TextureCreator* creator);
		~TextureManager();

		void setCreator(TextureCreator* creator);

		Texture* tryGetTexture(const char* filePath, const TextureType& textureType);

	private:
		TextureCreator* m_Creator;
		std::unordered_map<std::string, Texture*> m_TextureMap;
	};

	using TextureMgr = SG<TextureManager>;
}
