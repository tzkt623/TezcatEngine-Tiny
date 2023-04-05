#pragma once
#include "GLHead.h"

#include "Core/Renderer/Texture.h"
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

	class TINY_API GLTextureCube : public TextureCube
	{
	public:
		GLTextureCube();
		~GLTextureCube();

		void createTexture(const std::string& filePath) override;

	private:

	};

	class TINY_API GLTextureRenderBuffer2D : public TextureRenderBuffer2D
	{
	public:
		GLTextureRenderBuffer2D();
		virtual ~GLTextureRenderBuffer2D();

		void createTexture(const int& width, const int& high, const TextureChannel& internalChannel) override;

	};

	class TINY_API GLTextureBuffer2D : public TextureBuffer2D
	{
	public:
		GLTextureBuffer2D();
		virtual ~GLTextureBuffer2D();

		void createTexture(const int& width, const int& high, const TextureChannel& internalChannel) override;
		void createTexture(const int& width, const int& high, const TextureChannel& internalChannel, const TextureChannel& channel, const DataType& dataType) override;

	};




	//-----------------------------------
	//
	//
	//
	class TINY_API GLTextureCreator : public TextureCreator
	{
	public:
		GLTextureCreator() {}
		~GLTextureCreator() {}

		Texture* create(const std::string& filePath, const TextureType& textureType, const TextureFilter& filter, const TextureWrap& wrap) override;
	};
}
