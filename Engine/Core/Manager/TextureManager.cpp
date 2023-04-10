#include "TextureManager.h"
#include "../Renderer/Texture.h"
#include "../Utility/FileTool.h"
#include "../Data/Image.h"

using namespace tezcat::Tiny::Utility;
namespace tezcat::Tiny::Core
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
		std::unordered_map<std::string, std::string> files;
		FileTool::loadFiles(dir, files);

		for (auto& pair : files)
		{
			if (std::regex_search(pair.first, result, pattern))
			{
				std::cout << result[1] << result[2] << std::endl;

				// 				Image img;
				// 				img.openFile(pair.second);
				temp_name = result[2];
				if (temp_name == "R")
				{
					skybox_path_cache[result[1]][0] = pair.second;
					//					skybox_images[0] = &img;
				}
				else if (temp_name == "L")
				{
					skybox_path_cache[result[1]][1] = pair.second;
					//					skybox_images[1] = &img;
				}
				else if (temp_name == "U")
				{
					skybox_path_cache[result[1]][2] = pair.second;
					//					skybox_images[2] = &img;
				}
				else if (temp_name == "D")
				{
					skybox_path_cache[result[1]][3] = pair.second;
					//					skybox_images[3] = &img;
				}
				else if (temp_name == "F")
				{
					skybox_path_cache[result[1]][4] = pair.second;
					//					skybox_images[4] = &img;
				}
				else if (temp_name == "B")
				{
					skybox_path_cache[result[1]][5] = pair.second;
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
				img.openFile(pair.second, true);
				this->create2D(pair.first, img);
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

	TextureRenderBuffer2D* TextureManager::createBuffer2D(const int& width, const int& height, const TextureBufferInfo& info)
	{
		if (info.isManagered)
		{
			auto it = mTextureMap.try_emplace(info.name, nullptr);
			if (it.second)
			{
				auto tex = mCreator->createBuffer2D(width, height, info);
				tex->setSize(width, height);
				tex->setManagered(true);
				it.first->second = (Texture*)tex;
			}

			return (TextureRenderBuffer2D*)(it.first->second);
		}


		return mCreator->createBuffer2D(width, height, info);
	}

	TextureCube* TextureManager::createCube(const std::string& name, const std::array<Image, 6>& images)
	{
		auto it = mTextureMap.try_emplace(name, nullptr);
		if (it.second)
		{
			auto tex = mCreator->createCube(images, TextureInfo(name));
			tex->setManagered(true);
			it.first->second = (Texture*)tex;
		}

		return (TextureCube*)(it.first->second);
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

		auto it = mTextureMap.try_emplace(name, nullptr);
		if (it.second)
		{
			auto tex = mCreator->create2D(img, TextureInfo(name, channel));
			tex->setManagered(true);
			tex->setSize(img.getWidth(), img.getHeight());
			it.first->second = (Texture*)tex;
		}

		return (Texture2D*)(it.first->second);
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


	//------------------------------------------------------------
	//
	//	Creator
	//
	Texture2D* TextureCreator::create2D(const Image& img, const TextureInfo& info)
	{
		auto tex = this->create2D();
		tex->create(img, info);
		return tex;
	}

	TextureCube* TextureCreator::createCube(const std::array<Image, 6>& images, const TextureInfo& info)
	{
		auto tex = this->createCube();
		tex->create(images, info);
		return tex;
	}

	TextureBuffer2D* TextureCreator::createShowmap(const int& width, const int& height)
	{
		auto tex = this->createBuffer2D();
		tex->create(width, height,
			TextureBufferInfo("ShadowMap"
				, TextureBufferType::DepthComponent
				, TextureFilter::Nearest
				, TextureWrap::Repeat
				, TextureChannel::Depth
				, TextureChannel::Depth
				, DataType::Float32
				, false
				, true));
		tex->setSize(width, height);
		return tex;
	}

	TextureRenderBuffer2D* TextureCreator::createBuffer2D(const int& width, const int& height, const TextureBufferInfo& info)
	{
		if (info.isCache)
		{
			auto tex = this->createRenderBuffer2D();
			tex->create(width, height, info.internalChannel);
			tex->setBufferType(info.bufferType);
			return tex;
		}
		else
		{
			auto tex = this->createBuffer2D();
			tex->create(width, height, info.internalChannel, info.channel, info.dataType);
			tex->setBufferType(info.bufferType);
			return tex;
		}
	}

}
