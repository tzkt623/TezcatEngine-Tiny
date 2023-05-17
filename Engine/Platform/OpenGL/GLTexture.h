#pragma once
#include "GLHead.h"

#include "Core/Renderer/Texture.h"
#include "Core/Head/ConfigHead.h"
#include "Core/Manager/TextureManager.h"


namespace tezcat::Tiny::GL
{
	class TINY_API GLTexture2D : public Texture2D
	{
		GLTexture2D();

		TINY_Factory(GLTexture2D);
		TINY_RTTI_H(GLTexture2D);
	public:
		virtual ~GLTexture2D();

		void create(const Image& img, const TextureInfo& info) override;
		void create(const int& width
				   , const int& height
				   , const TextureChannel& internalChannel
				   , const TextureChannel& channel
				   , const DataType& dataType) override;

		void create(const int& width, const int& height, const TextureInfo& info) override;
	};

	class TINY_API GLTextureCube : public TextureCube
	{
		GLTextureCube();

		TINY_Factory(GLTextureCube);
		TINY_RTTI_H(GLTextureCube);
	public:
		virtual ~GLTextureCube();

		void create(const std::array<Image, 6>& images, const TextureInfo& info) override;
		void create(const int& width, const int& hegiht, const TextureInfo& info) override;

	private:

	};

	class TINY_API GLTextureRender2D : public TextureRender2D
	{
		GLTextureRender2D();
		TINY_Factory(GLTextureRender2D);
		TINY_RTTI_H(GLTextureRender2D);
	public:
		virtual ~GLTextureRender2D();

		void create(const int& width
			, const int& height
			, const TextureChannel& internalChannel) override;
	};


	//-----------------------------------
	//
	//
	//
	class TINY_API GLTextureCreator : public TextureCreator
	{
	public:
		GLTextureCreator() {}
		virtual ~GLTextureCreator() {}


	protected:
		Texture2D* create2D() override;
		TextureCube* createCube() override;
		TextureRender2D* createRender2D() override;
	};
}
