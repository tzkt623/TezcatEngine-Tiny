#pragma once
#include "Core/Renderer/Texture.h"
#include "GLHead.h"
#include "Core/Head/ConfigHead.h"
#include "Core/Manager/TextureManager.h"


namespace tezcat::Tiny::Core
{
	class TINY_API GLTexture2D : public Texture2D
	{
	public:
		GLTexture2D();
		virtual ~GLTexture2D();

		void createTexture(Image* image) override;
	};

	class GLTextureCreator : public TextureCreator
	{
	public:
		GLTextureCreator() {}
		~GLTextureCreator() {}

		Texture* create(const char* filePath, const TextureType& textureType) override;

	};
}