#pragma once
#include "../Head/TinyCpp.h"



namespace tezcat::Tiny
{
	class Texture;
	class Texture2D;
	class TextureCube;
	class TextureRender2D;

	class TINY_API TextureManager
	{
		TextureManager() = delete;
		~TextureManager() = delete;
	public:

		static void loadResource(const std::string& dir);

		static std::tuple<bool, Texture2D*> create2D(std::string name);
		static std::tuple<bool, TextureCube*> createCube(std::string name);
		static std::tuple<bool, TextureRender2D*> createRender2D(std::string name);

	public:
		static Texture* find(const std::string& name);

		template<class Type>
		static Type* find(const std::string& name)
		{
			auto it = mTextureMap.find(name);
			if (it != mTextureMap.end())
			{
				return (Type*)it->second;
			}

			return nullptr;
		}


		static void outputAll2DHDR(std::vector<Texture2D*>& container);

	private:
		static void add(const std::string_view& name, Texture* tex);


	private:
		static std::unordered_map<std::string_view, Texture*> mTextureMap;
	};
}

