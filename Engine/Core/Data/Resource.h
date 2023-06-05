#pragma once

#include "../Head/TinyCpp.h"
#include "../Renderer/Texture.h"
#include "../Manager/TextureManager.h"
#include "../Renderer/BaseGraphics.h"
#include "../Shader/Shader.h"

#include "ThirdParty/Hash/city.h"

#include "Image.h"

namespace tezcat::Tiny
{
	class TinyObject;
	class Texture2D;

	struct ResourcePackage
	{

	};

	class Resource
	{
		struct Wrapper
		{

		};

		Resource() = delete;
		~Resource() = delete;
	public:

		template<class Data>
		static Data* load(const std::string& path)
		{
			TinyThrow_Logic("");
			return nullptr;
		}

		template<class Data>
		static Data* loadOnly(const std::string& path)
		{
			TinyThrow_Logic("");
			return nullptr;
		}

		/*
		template<>
		static Texture2D* load<>(const std::string& path)
		{
			auto id = CityHash64(path.c_str(), path.size());
			auto result = mDataUSet.try_emplace(id, nullptr);
			if (result.second)
			{
				Image img;
				img.openFile(path);
				auto tex = TextureMgr::getInstance()->create2D("", img);
				tex->addRef();
				result.first->second = tex;
			}

			return (Texture2D*)result.first->second;
		}
		*/

		template<>
		static Texture2D* load<>(const std::string& path)
		{
			std::filesystem::path file_path(FileTool::getRootRelativeResDir() + "/" + path);
			Image* img = Image::create();
			img->openFile(file_path.string());

			Texture2D* t2d = Texture2D::create(file_path.filename().string());
			t2d->setData(img);
			t2d->generate();
			t2d->addRef();

			mDataUSet.emplace(t2d);
			return t2d;
		}

		template<>
		static Texture2D* loadOnly<>(const std::string& path)
		{
			std::filesystem::path file_path(FileTool::getRootRelativeResDir() + "/" + path);
			Image* img = Image::create();
			img->openFile(file_path.string());

			Texture2D* t2d = Texture2D::create(file_path.filename().string());
			t2d->setData(img);
			t2d->generate();
			return t2d;
		}

		template<>
		static Shader* load<>(const std::string& path)
		{
			Shader* shader = Shader::create(FileTool::getRootRelativeResDir() + "/" + path);
			shader->generate();
			shader->addRef();

			mDataUSet.emplace(shader);
			return shader;
		}

		template<>
		static Image* load<>(const std::string& path)
		{
			Image* image = Image::create();
			if (image->openFile(FileTool::getRootRelativeResDir() + "/" + path, true))
			{
				image->addRef();
				mDataUSet.emplace(image);
				return image;
			}

			return nullptr;
		}

		template<class Data>
		static void unload(Data* data)
		{
			mDataUSet.erase(data);
			data->subRef();
		}

		template<>
		static void unload<>(Shader* data)
		{
			mDataUSet.erase(data);
			data->subRef();
		}

		template<>
		static void unload<>(Texture2D* data)
		{
			mDataUSet.erase(data);
			data->subRef();
		}

		template<>
		static void unload<>(Image* data)
		{
			mDataUSet.erase(data);
			data->subRef();
		}


		static void unloadUnused()
		{
			auto it = mDataUSet.begin();
			auto end = mDataUSet.end();
			while (it != end)
			{
				if ((*it)->refCount() == 1)
				{
					(*it)->subRef();
					it = mDataUSet.erase(it);
					end = mDataUSet.end();
				}
				else
				{
					++it;
				}
			}
		}

	private:
		static std::unordered_set<TinyObject*> mDataUSet;
		static std::unordered_map<std::string, Texture2D*> mTexture2DUSet;
		static std::unordered_map<std::string, Shader*> mShaderUSet;
		static std::unordered_map<std::string, Image*> mImageUSet;
	};
}
