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

	class TINY_API ResourceManager
	{
		ResourceManager() = delete;
		~ResourceManager() = delete;
	public:
		static void init(const std::string& folderPath);
		static const std::string& getEngineDir() { return sEngineDir; }
		static const std::string& getResDir() { return sResDir; }

		static const std::string& getRelativeEngineDir() { return sRelativeEngineDir; }
		static const std::string& getRelativeResDir() { return sRelativeResDir; }

		template<class Data>
		static Data* loadAndSave(const file_path& relativePath)
		{
			TINY_THROW_LOGIC("");
			return nullptr;
		}

		template<class Data>
		static Data* loadOnly(const file_path& relativePath)
		{
			TINY_THROW_LOGIC("");
			return nullptr;
		}

		template<>
		static Texture2D* loadAndSave<>(const file_path& relativePath)
		{
			file_path generic_relative_path = file_sys_helper::generic(relativePath);

			auto it = mTexture2DUMap.find(generic_relative_path);
			if (it == mTexture2DUMap.end())
			{
				auto image = loadAndSave<Image>(generic_relative_path);
				if (image)
				{
					file_path file_path(sRelativeResDir / generic_relative_path);

					Texture2D* t2d = Texture2D::create(file_path.filename().string());
					t2d->setImage(image);
					t2d->generate();
					t2d->saveObject();

					mTexture2DUMap.emplace(generic_relative_path, t2d);
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
		static Texture2D* loadOnly<>(const file_path& relativePath)
		{
			file_path generic_relative_path = file_sys_helper::generic(relativePath);

			file_path file_path(sRelativeResDir / generic_relative_path);
			Image* img = Image::create();
			if (img->openFile(file_path.string(), true))
			{
				Texture2D* t2d = Texture2D::create(generic_relative_path.filename().string());
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
		static Image* loadAndSave<>(const file_path& relativePath)
		{
			file_path generic_relative_path = file_sys_helper::generic(relativePath);

			auto result = mImageMap.try_emplace(generic_relative_path, nullptr);
			if (result.second)
			{
				Image* image = Image::create();
				if (image->openFile(sRelativeResDir / generic_relative_path, true))
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
		static Image* loadOnly<>(const file_path& relativePath)
		{
			file_path generic_relative_path = file_sys_helper::generic(relativePath);

			Image* image = Image::create();
			if (image->openFile(sRelativeResDir / generic_relative_path, true))
			{
				return image;
			}

			return nullptr;
		}


		template<>
		static Model* loadAndSave<>(const file_path& relativePath)
		{
			file_path generic_relative_path = file_sys_helper::generic(relativePath);

			auto result = mModelMap.try_emplace(generic_relative_path, nullptr);
			if (result.second)
			{
				Model* model = Model::create();
				model->load(sRelativeResDir / generic_relative_path);
				model->saveObject();
				model->setPath(result.first->first);
				result.first->second = model;
			}

			return result.first->second;
		}

		template<>
		static Model* loadOnly<>(const file_path& relativePath)
		{
			file_path generic_relative_path = file_sys_helper::generic(relativePath);

			Model* model = Model::create();
			model->load(sRelativeResDir / generic_relative_path);
			return model;
		}

		template<class Data>
		static Data* loadDefault(const file_path& path)
		{
			throw std::runtime_error("Resource Error");
		}

		template<>
		static Image* loadDefault<Image>(const file_path& relativePath)
		{
			file_path generic_relative_path = file_sys_helper::generic(relativePath);

			auto result = mTinyImageMap.try_emplace(generic_relative_path, nullptr);
			if (result.second)
			{
				Image* image = Image::create();
				if (image->openFile(sRelativeResDir / generic_relative_path, true))
				{
					image->saveObject();
					result.first->second = image;
					return image;
				}
			}

			return result.first->second;
		}

		template<>
		static Texture2D* loadDefault<Texture2D>(const file_path& relativePath)
		{
			return loadAndSave<Texture2D>(relativePath);
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
	};
}
