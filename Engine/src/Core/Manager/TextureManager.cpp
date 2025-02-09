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

#include "Core/Manager/TextureManager.h"
#include "Core/Renderer/Texture.h"
#include "Core/Tool/Tool.h"
#include "Core/Data/Image.h"
#include "Core/Head/RenderConfig.h"

namespace tezcat::Tiny
{
	std::unordered_map<std::string_view, Texture*> TextureManager::mTextureMap;

	void TextureManager::loadResource(const std::string& dir)
	{
		std::regex pattern(R"((skybox_\w+)_(\w+))");
		std::smatch result;
		std::array<Image*, 6> skybox_images;
		std::unordered_map<std::string, std::array<std::string, 6>> skybox_path_cache;

		std::string temp_name;
		FileInfoMap files;
		FileTool::loadFiles(dir, files);

		for (auto& pair : files)
		{
			auto& info = pair.second;

			if (std::regex_search(info.name, result, pattern))
			{
				std::cout << result[1] << result[2] << std::endl;

				// 				Image img;
				// 				img.openFile(pair.second);
				temp_name = result[2];
				if (temp_name == "R")
				{
					skybox_path_cache[result[1]][0] = std::move(info.path);
					//					skybox_images[0] = &img;
				}
				else if (temp_name == "L")
				{
					skybox_path_cache[result[1]][1] = std::move(info.path);
					//					skybox_images[1] = &img;
				}
				else if (temp_name == "U")
				{
					skybox_path_cache[result[1]][2] = std::move(info.path);
					//					skybox_images[2] = &img;
				}
				else if (temp_name == "D")
				{
					skybox_path_cache[result[1]][3] = std::move(info.path);
					//					skybox_images[3] = &img;
				}
				else if (temp_name == "F")
				{
					skybox_path_cache[result[1]][4] = std::move(info.path);
					//					skybox_images[4] = &img;
				}
				else if (temp_name == "B")
				{
					skybox_path_cache[result[1]][5] = std::move(info.path);
					//					skybox_images[5] = &img;
				}
				else
				{
					throw std::logic_error("Load Skybox Image Error");
				}

				//				this->createCube(result[1], skybox_images);
			}
			else
			{
				Image* image = Image::create();
				image->openFile(info, true);
				Texture2D* t2d = Texture2D::create(info.name);
				t2d->setImage(image);
				t2d->generate();

				add(t2d->getName(), t2d);
			}
		}

		for (auto& pair : skybox_path_cache)
		{
			for (int i = 0; i < 6; i++)
			{
				Image* image = Image::create();
				image->openFile(pair.second[i]);
				skybox_images[i] = image;
			}

			TextureCube* cube = TextureCube::create(pair.first);
			cube->setImage(skybox_images);
			cube->generate();

			add(cube->getName(), cube);
		}
	}


	Texture* TextureManager::find(const std::string& name)
	{
		auto it = mTextureMap.find(name);
		if (it != mTextureMap.end())
		{
			return it->second;
		}

		return nullptr;
	}

	void TextureManager::outputAll2DHDR(std::vector<Texture2D*>& container)
	{
		for (auto& pair : mTextureMap)
		{
			if (pair.second->getTextureType() == TextureType::Texture2D)
			{
				auto type = static_cast<Texture2D*>(pair.second)->getInternalFormat().tiny;
				if (type == TextureInternalFormat::RGB16F)
				{
					container.push_back(static_cast<Texture2D*>(pair.second));
				}
			}
		}
	}

	void TextureManager::add(const std::string_view& name, Texture* tex)
	{
		auto result = mTextureMap.try_emplace(name, tex);
		if (result.second)
		{
			tex->saveObject();
		}
	}

	std::tuple<bool, Texture2D*> TextureManager::create2D(std::string name)
	{
		auto result = mTextureMap.find(name);
		if (result != mTextureMap.end())
		{
			return { false, (Texture2D*)result->second };
		}

		auto t = Texture2D::create();
		t->setName(name);
		t->saveObject();
		mTextureMap.emplace(t->getName(), t);
		return { true, t };

	}

	std::tuple<bool, TextureCube*> TextureManager::createCube(std::string name)
	{
		auto result = mTextureMap.find(name);
		if (result != mTextureMap.end())
		{
			return { false, (TextureCube*)result->second };
		}

		auto t = TextureCube::create();
		t->setName(name);
		t->saveObject();
		mTextureMap.emplace(t->getName(), t);
		return { true, t };
	}

	std::tuple<bool, TextureRender2D*> TextureManager::createRender2D(std::string name)
	{
		auto result = mTextureMap.find(name);
		if (result != mTextureMap.end())
		{
			return { false, (TextureRender2D*)result->second };
		}

		auto t = TextureRender2D::create();
		t->setName(name);
		t->saveObject();
		mTextureMap.emplace(t->getName(), t);
		return { true, t };
	}
}
