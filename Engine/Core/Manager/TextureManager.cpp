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
		std::array<Image, 6> skybox_images;
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
				Image img;
				img.openFile(info, true);
				this->create2D(info.name, img);
			}
		}

		for (auto& pair : skybox_path_cache)
		{
			for (int i = 0; i < 6; i++)
			{
				Image image;
				image.openFile(pair.second[i]);
				skybox_images[i] = std::move(image);
			}

			this->createCube(pair.first, skybox_images);
		}
	}

	Texture2D* TextureManager::create2D(const int& width, const int& height, const TextureInfo& info)
	{
		if (!info.name.empty())
		{
			auto it = mTextureMap.tryEmplace(info.name, [&]()
			{
				auto tex = mCreator->create2D(width, height, info);
				tex->setName(info.name);
				return tex;

			});
			return (Texture2D*)(it.first->second);
		}


		return mCreator->create2D(width, height, info);
	}

	TextureCube* TextureManager::createCube(const std::string& name, const std::array<Image, 6>& images)
	{
		auto it = mTextureMap.tryEmplace(name, [&]()
		{
			auto tex = mCreator->createCube(images, TextureInfo(name));
			tex->setName(name);
			return tex;
		});

		return (TextureCube*)(it.first->second);
	}

	TextureCube* TextureManager::createCube(const int& width, const int& height, const TextureInfo& info)
	{
		if (!info.name.empty())
		{
			auto it = mTextureMap.tryEmplace(info.name, [&]()
			{
				auto tex = mCreator->createCube(width, height, info);
				tex->setName(info.name);
				return tex;
			});
			return (TextureCube*)(it.first->second);
		}

		return mCreator->createCube(width, height, info);
	}

	Texture2D* TextureManager::create2D(const std::string& name, const Image& img)
	{
		TextureChannel channel;
		switch (img.getChannels())
		{
		case 1: channel = TextureChannel::R; break;
		case 2: channel = TextureChannel::RG; break;
		case 3: channel = TextureChannel::RGB; break;
		case 4: channel = TextureChannel::RGBA; break;
		default:
			break;
		}

		TextureInfo info(name, channel);

		if (img.isHDR())
		{
			info.internalChannel = TextureChannel::RGB16f;
			info.dataType = DataType::Float32;
		}

		auto it = mTextureMap.tryEmplace(info.name, [&]()
		{
			auto tex = mCreator->create2D(img, info);
			tex->setName(name);
			return tex;
		});

		return (Texture2D*)(it.first->second);
	}

	TextureRender2D* TextureManager::createRender2D(const int& width, const int& height, const TextureInfo& info)
	{
		if (!info.name.empty())
		{
			auto it = mTextureMap.tryEmplace(info.name, [&]()
			{
				auto tex = mCreator->createRender2D(width, height, info);
				tex->setName(info.name);
				return tex;
			});

			return (TextureRender2D*)(it.first->second);
		}

		return mCreator->createRender2D(width, height, info);
	}

	Texture* TextureManager::findTexture(const std::string& name)
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

	//------------------------------------------------------------
	//
	//	Creator
	//
	Texture2D* TextureCreator::create2D(const Image& img, const TextureInfo& info)
	{
		auto tex = this->create2D();
		tex->create(img, info);
		tex->setAttachPosition(info.attachPosition);
		return tex;
	}

	TextureCube* TextureCreator::createCube(const std::array<Image, 6>& images, const TextureInfo& info)
	{
		auto tex = this->createCube();
		tex->create(images, info);
		tex->setAttachPosition(info.attachPosition);
		return tex;
	}

	TextureCube* TextureCreator::createCube(const int& width, const int& height, const TextureInfo& info)
	{
		auto tex = this->createCube();
		tex->create(width, height, info);
		tex->setAttachPosition(info.attachPosition);
		return tex;
	}

	Texture2D* TextureCreator::create2D(const int& width, const int& height, const TextureInfo& info)
	{
		auto tex = this->create2D();
		tex->create(width, height, info.internalChannel, info.channel, info.dataType);
		tex->setAttachPosition(info.attachPosition);
		return tex;
	}

	TextureRender2D* TextureCreator::createRender2D(const int& width, const int& height, const TextureInfo& info)
	{
		auto tex = this->createRender2D();
		tex->create(width, height, info.internalChannel);
		tex->setAttachPosition(info.attachPosition);
		return tex;
	}
}
