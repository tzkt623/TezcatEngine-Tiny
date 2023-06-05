#include "TextureManager.h"
#include "../Renderer/Texture.h"
#include "../Tool/Tool.h"
#include "../Data/Image.h"

namespace tezcat::Tiny
{
	TextureManager::TextureManager()
	{
		SG<TextureManager>::attach(this);
	}

	TextureManager::~TextureManager()
	{

	}

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
				t2d->setData(image);
				t2d->generate();

				this->add(t2d->getName(), t2d);
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
			cube->setData(skybox_images);
			cube->generate();

			this->add(cube->getName(), cube);
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
				auto type = static_cast<Texture2D*>(pair.second)->getInternalChannel().tiny;
				if (type == TextureChannel::RGB16f)
				{
					container.push_back(static_cast<Texture2D*>(pair.second));
				}
			}
		}
	}

	void TextureManager::add(const std::string& name, Texture* tex)
	{
		auto result = mTextureMap.try_emplace(name, tex);
		tex->addRef();
		if (!result.second)
		{
			result.first->second->subRef();
			result.first->second = tex;
		}
	}

	Texture2D* TextureManager::create2D(const std::string& name)
	{
		auto result = mTextureMap.try_emplace(name, nullptr);
		if (result.second)
		{
			auto t = Texture2D::create(name);
			t->addRef();
			result.first->second = t;
		}
		return (Texture2D*)result.first->second;
	}

	TextureCube* TextureManager::createCube(const std::string& name)
	{
		auto result = mTextureMap.try_emplace(name, nullptr);
		if (result.second)
		{
			auto t = TextureCube::create(name);
			t->addRef();
			result.first->second = t;
		}
		return (TextureCube*)result.first->second;
	}

	TextureRender2D* TextureManager::createRender2D(const std::string& name)
	{
		auto result = mTextureMap.try_emplace(name, nullptr);
		if (result.second)
		{
			auto t = TextureRender2D::create(name);
			t->addRef();
			result.first->second = t;
		}
		return (TextureRender2D*)result.first->second;
	}

}
