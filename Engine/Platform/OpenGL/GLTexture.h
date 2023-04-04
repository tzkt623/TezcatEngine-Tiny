#pragma once
#include "Core/Renderer/Texture.h"
#include "GLHead.h"
#include "Core/Head/ConfigHead.h"
#include "Core/Manager/TextureManager.h"


namespace tezcat::Tiny::GL
{
	class TINY_API GLTexture2D : public Texture2D
	{
	public:
		GLTexture2D();
		virtual ~GLTexture2D();

		void createTexture(Image* image) override;
	};

	class GLTextureCube : public TextureCube
	{
	public:
		GLTextureCube();
		~GLTextureCube();

		void createTexture(const std::string& filePath) override;

	private:

	};



	class TINY_API GLTextureCreator : public TextureCreator
	{
	public:
		GLTextureCreator() {}
		~GLTextureCreator() {}

		Texture* create(const std::string& filePath, const TextureType& textureType, const TextureFilter& filter, const TextureWrap& wrap) override;
	};
}
