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

		TINY_Factory(GLTexture2D)
		TINY_RTTI_H(GLTexture2D)
	public:
		virtual ~GLTexture2D();

		void create(const Image& img, const TextureInfo& info) override;
	};

	class TINY_API GLTextureCube : public TextureCube
	{
		GLTextureCube();

		TINY_Factory(GLTextureCube)
		TINY_RTTI_H(GLTextureCube)
	public:
		virtual ~GLTextureCube();

		void create(const std::array<Image, 6>& images, const TextureInfo& info) override;

	private:

	};

	class TINY_API GLTextureRenderBuffer2D : public TextureRenderBuffer2D
	{
		GLTextureRenderBuffer2D();
		TINY_Factory(GLTextureRenderBuffer2D)
		TINY_RTTI_H(GLTextureRenderBuffer2D)
	public:
		virtual ~GLTextureRenderBuffer2D();

		void create(const int& width
			, const int& high
			, const TextureChannel& internalChannel) override;
	};

	class TINY_API GLTextureBuffer2D : public TextureBuffer2D
	{
		GLTextureBuffer2D();
		TINY_Factory(GLTextureBuffer2D)
		TINY_RTTI_H(GLTextureBuffer2D)
	public:
		virtual ~GLTextureBuffer2D();

		void create(const int& width
			, const int& high
			, const TextureChannel& internalChannel) override;

		void create(const int& width
			, const int& high
			, const TextureChannel& internalChannel
			, const TextureChannel& channel
			, const DataType& dataType) override;

		void create(const int& width, const int& high
			, const TextureBufferInfo& info) override;
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
		TextureRenderBuffer2D* createRenderBuffer2D() override;
		TextureBuffer2D* createBuffer2D() override;
	};
}
