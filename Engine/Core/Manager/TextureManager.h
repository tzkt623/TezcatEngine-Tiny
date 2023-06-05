#pragma once
#include "../Manager/Manager.h"
#include "../Head/TinyCpp.h"
#include "../Head/RenderConfig.h"

#include "../Tool/Tool.h"


namespace tezcat::Tiny
{
	class Texture;
	class Texture2D;
	class TextureCube;
	class TextureRender2D;

	class TINY_API TextureManager
	{
	public:
		TextureManager();
		virtual ~TextureManager();
		void loadResource(const std::string& dir);

		Texture2D* create2D(const std::string& name);
		TextureCube* createCube(const std::string& name);
		TextureRender2D* createRender2D(const std::string& name);

	public:
		void add(const std::string& name, Texture* tex);
		Texture* find(const std::string& name);
		void outputAll2DHDR(std::vector<Texture2D*>& container);

	private:
		TinyUMap<std::string, Texture*> mTextureMap;
	};

	using TextureMgr = SG<TextureManager>;
}

