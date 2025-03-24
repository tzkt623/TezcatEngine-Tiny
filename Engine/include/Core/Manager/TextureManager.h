#pragma once

/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
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

		static bool remove(std::string name);

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

