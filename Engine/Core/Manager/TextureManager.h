#pragma once
#include "../Manager/Manager.h"
#include "../Head/TinyCpp.h"
#include "../Head/RenderConfig.h"

#include "../Tool/Tool.h"


namespace tezcat::Tiny
{
	class Image;
	class Texture;
	class Texture2D;
	class TextureCube;
	class TextureRender2D;
	struct TextureInfo;
	struct TextureInfo;
	class TINY_API TextureCreator
	{
	public:
		TextureCreator() {}
		virtual ~TextureCreator() {}

	public:
		Texture2D* create2D(const int& width, const int& height, const TextureInfo& info);
		Texture2D* create2D(const Image& img, const TextureInfo& info);

		TextureCube* createCube(const std::array<Image, 6>& images, const TextureInfo& info);
		TextureCube* createCube(const int& width, const int& height, const TextureInfo& info);

		TextureRender2D* createRender2D(const int& width, const int& height, const TextureInfo& info);

	protected:
		virtual Texture2D* create2D() = 0;
		virtual TextureCube* createCube() = 0;
		virtual TextureRender2D* createRender2D() = 0;

	};

	class TINY_API TextureManager : public Manager<TextureCreator>
	{
	public:
		TextureManager();
		virtual ~TextureManager();
		void loadResource(const std::string& dir);

	public:
		TextureCube* createCube(const std::string& name, const std::array<Image, 6>& images);
		TextureCube* createCube(const int& width, const int& height, const TextureInfo& info);
		Texture2D* create2D(const std::string& name, const Image& img);
		Texture2D* create2D(const int& width, const int& height, const TextureInfo& info);
		TextureRender2D* createRender2D(const int& width, const int& height, const TextureInfo& info);

	public:
		Texture* findTexture(const std::string& name);
		void outputAll2DHDR(std::vector<Texture2D*>& container);

	private:
		TinyUMap<std::string, Texture*> mTextureMap;
	};

	using TextureMgr = SG<TextureManager>;


}

