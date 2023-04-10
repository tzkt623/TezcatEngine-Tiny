#pragma once
#include "../Manager/Manager.h"
#include "../Head/CppHead.h"
#include "../Head/Context.h"

#include "Utility/Utility.h"

namespace tezcat::Tiny::Core
{
	class Image;
	class Texture;
	class Texture2D;
	class TextureCube;
	class TextureRenderBuffer2D;
	class TextureBuffer2D;
	struct TextureInfo;
	struct TextureBufferInfo;
	class TINY_API TextureCreator
	{
	public:
		TextureCreator() {}
		virtual ~TextureCreator() {}

	public:
		TextureRenderBuffer2D* createBuffer2D(const int& width, const int& height, const TextureBufferInfo& info);
		Texture2D* create2D(const Image& img, const TextureInfo& info);
		TextureCube* createCube(const std::array<Image, 6>& images, const TextureInfo& info);
		TextureBuffer2D* createShowmap(const int& width, const int& height);

	protected:
		virtual Texture2D* create2D() = 0;
		virtual TextureCube* createCube() = 0;
		virtual TextureRenderBuffer2D* createRenderBuffer2D() = 0;
		virtual TextureBuffer2D* createBuffer2D() = 0;

	};

	class TINY_API TextureManager : public Manager<TextureCreator>
	{
	public:
		TextureManager();
		virtual ~TextureManager();
		void loadResource(const std::string& dir);

	public:
		TextureCube* createCube(const std::string& name, const std::array<Image, 6>& images);
		Texture2D* create2D(const std::string& name, const Image& img);
		TextureRenderBuffer2D* createBuffer2D(const int& width, const int& height, const TextureBufferInfo& info);

	public:
		Texture* findTexture(const std::string& name);

	private:
		std::unordered_map<std::string, Texture*> mTextureMap;
	};

	using TextureMgr = SG<TextureManager>;
}

