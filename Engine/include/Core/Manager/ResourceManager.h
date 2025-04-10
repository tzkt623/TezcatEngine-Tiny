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
#include "../Data/Image.h"

#include "../Head/TinyCpp.h"
#include "../Renderer/Texture.h"
#include "../Manager/TextureManager.h"
#include "../Renderer/BaseGraphics.h"
#include "../Shader/Shader.h"
#include "../Data/MeshData.h"

#include "ThirdParty/Hash/city.h"


namespace tezcat::Tiny
{
	class TinyObject;
	class Texture2D;
	class Mode;
	class Image;

	template<typename T>
	struct ResourceSelector
	{
	};

	template<>
	struct ResourceSelector<Mode>
	{
		constexpr static uint32_t ID = 1;
	};

	template<>
	struct ResourceSelector<Texture>
	{
		constexpr static uint32_t ID = 2;
	};

	template<>
	struct ResourceSelector<Shader>
	{
		constexpr static uint32_t ID = 3;
	};

#define true_path true_path

	class TINY_API ResourceManager
	{
		ResourceManager() = delete;
		~ResourceManager() = delete;

		enum class PathType : int8_t
		{
			Relative,
			Absolute
		};

	public:
		static void init();
		static const std::string& getEngineDir() { return sEngineDir; }
		static const std::string& getResDir() { return sResDir; }

		static const std::string& getRelativeEngineDir() { return sRelativeEngineDir; }
		static const std::string& getRelativeResDir() { return sRelativeResDir; }

		static file_path clearPath(const file_path& path)
		{
			//说明是加载
			//if (path.is_relative())
			//{
			//	return file_sys_helper::generic(sRelativeResDir / path);
			//}

			return file_sys_helper::generic(path);
		}

		template<class Data>
		static Data* loadAndSave(const file_path& path)
		{
			TINY_THROW_LOGIC("");
			return nullptr;
		}

		template<class Data>
		static Data* loadOnly(const file_path& path)
		{
			TINY_THROW_LOGIC("");
			return nullptr;
		}

		static void loadMetaFile(const file_path& path);

		template<>
		static Texture2D* loadAndSave<>(const file_path& path)
		{
			auto true_path = clearPath(path);

			auto it = mTexture2DUMap.find(true_path);
			if (it == mTexture2DUMap.end())
			{
				auto image = loadAndSaveImageHelper(true_path);
				if (image)
				{
					Texture2D* t2d = Texture2D::create(true_path.filename().string());
					t2d->setImage(image);
					t2d->generate();
					t2d->saveObject();

					mTexture2DUMap.emplace(true_path, t2d);
					return t2d;
				}

				return nullptr;
			}
			else
			{
				return it->second;
			}
		}

		template<>
		static Texture2D* loadOnly<>(const file_path& path)
		{
			auto true_path = clearPath(path);
			Image* img = Image::create();
			if (img->openFile(true_path, true))
			{
				Texture2D* t2d = Texture2D::create(true_path.filename().string());
				t2d->setImage(img);
				t2d->generate();
				return t2d;
			}

			return nullptr;
		}

		//template<>
		//static Shader* loadAndSave<>(const file_path& path)
		//{
		//	Shader* shader = Shader::create(std::format("{}/{}", sRelativeResDir, path));
		//	shader->generate();
		//	shader->saveObject();
		//
		//	mDataUSet.emplace(shader);
		//	return shader;
		//}
		//
		//template<>
		//static Shader* loadOnly<>(const file_path& path)
		//{
		//	Shader* shader = Shader::create(std::format("{}/{}", sRelativeResDir, path));
		//	shader->generate();
		//	return shader;
		//}



		template<>
		static Image* loadAndSave<>(const file_path& path)
		{
			auto true_path = clearPath(path);

			auto result = mImageMap.try_emplace(true_path, nullptr);
			if (result.second)
			{
				Image* image = Image::create();
				if (image->openFile(true_path, true))
				{
					image->saveObject();
					result.first->second = image;
					return image;
				}
				else
				{
					return nullptr;
				}
			}

			return result.first->second;
		}

		template<>
		static Image* loadOnly<>(const file_path& path)
		{
			auto true_path = clearPath(path);

			Image* image = Image::create();
			if (image->openFile(true_path, true))
			{
				return image;
			}

			return nullptr;
		}


		template<>
		static Model* loadAndSave<>(const file_path& path)
		{
			auto true_path = clearPath(path);

			auto result = mModelMap.try_emplace(true_path, nullptr);
			if (result.second)
			{
				Model* model = Model::create();
				model->load(true_path);
				model->saveObject();
				model->setPath(result.first->first);
				result.first->second = model;
			}

			return result.first->second;
		}

		template<>
		static Model* loadOnly<>(const file_path& path)
		{
			auto true_path = clearPath(path);

			Model* model = Model::create();
			model->load(true_path);
			return model;
		}

		template<class Data>
		static Data* loadDefault(const file_path& path)
		{
			throw std::runtime_error("Resource Error");
		}

		template<>
		static Image* loadDefault<Image>(const file_path& path)
		{
			auto true_path = clearPath(path);

			auto result = mTinyImageMap.try_emplace(true_path, nullptr);
			if (result.second)
			{
				Image* image = Image::create();
				if (image->openFile(true_path, true))
				{
					image->saveObject();
					result.first->second = image;
					return image;
				}
			}

			return result.first->second;
		}

		template<>
		static Texture2D* loadDefault<Texture2D>(const file_path& path)
		{
			return loadAndSave<Texture2D>(path);
		}

#pragma region Unload
		template<class Data>
		static void unload(Data* data)
		{
			auto result = mDataUSet.erase(data);
			data->subRef();
		}

		template<>
		static void unload<>(Shader* data)
		{
			mDataUSet.erase(data);
			data->deleteObject();
		}

		template<>
		static void unload<>(Texture2D* data)
		{
			mDataUSet.erase(data);
			data->deleteObject();
		}

		template<>
		static void unload<>(Image* data)
		{
			mDataUSet.erase(data);
			data->deleteObject();
		}

		template<>
		static void unload<>(Model* data)
		{
			mDataUSet.erase(data);
			data->deleteObject();
		}


		static void unloadUnused()
		{
			unloadUnusedSet(mDataUSet);
			unloadUnusedMap(mModelMap);
			unloadUnusedMap(mImageMap);
			unloadUnusedMap(mTexture2DUMap);
		}

	private:
		template<class Set>
		static void unloadUnusedSet(Set& container)
		{
			auto it = container.begin();
			while (it != container.end())
			{
				if ((*it)->getRefCount() == 1)
				{
					(*it)->deleteObject();
					it = container.erase(it);
				}
				else
				{
					++it;
				}
			}
		}


		template<class Map>
		static void unloadUnusedMap(Map& container)
		{
			auto it = container.begin();
			while (it != container.end())
			{
				auto ptr = it->second;
				if (ptr->getRefCount() == 1)
				{
					ptr->deleteObject();
					it = container.erase(it);
				}
				else
				{
					++it;
				}
			}
		}

		static Image* loadAndSaveImageHelper(const file_path& path)
		{
			auto result = mImageMap.try_emplace(path, nullptr);
			if (result.second)
			{
				Image* image = Image::create();
				if (image->openFile(path, true))
				{
					image->saveObject();
					result.first->second = image;
					return image;
				}
				else
				{
					return nullptr;
				}
			}

			return result.first->second;
		}

#pragma endregion

	private:
		static std::unordered_set<TinyObject*> mDataUSet;
		static std::unordered_map<std::string_view, Shader*> mShaderUSet;

		static std::unordered_map<file_path, Texture2D*> mTexture2DUMap;
		static std::unordered_map<file_path, Image*> mImageMap;
		static std::unordered_map<file_path, Model*> mModelMap;

		static std::unordered_map<file_path, Image*> mTinyImageMap;


	private:
		static std::string sEngineDir;
		static std::string sResDir;

		static std::string sRelativeEngineDir;
		static std::string sRelativeResDir;

	private:
		static std::unordered_map<std::string, std::function<void(std::string&)>> mMetaLoadMap;
	};
}
