#pragma once
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
		static Data* loadAndSave(const std::string& path)
		{
			TINY_THROW_LOGIC("");
			return nullptr;
		}

		template<class Data>
		static Data* loadOnly(const std::string& path)
		{
			TINY_THROW_LOGIC("");
			return nullptr;
		}

		template<>
		static Texture2D* loadAndSave<>(const std::string& path)
		{
			std::filesystem::path file_path(sRelativeResDir + "/" + path);
			Image* img = Image::create();
			if (img->openFile(file_path.string(), true))
			{
				Texture2D* t2d = Texture2D::create(file_path.filename().string());
				t2d->setImage(img);
				t2d->generate();
				t2d->saveObject();

				mDataUSet.emplace(t2d);
				return t2d;
			}

			return nullptr;
		}

		template<>
		static Texture2D* loadOnly<>(const std::string& path)
		{
			std::filesystem::path file_path(sRelativeResDir + "/" + path);
			Image* img = Image::create();
			if (img->openFile(file_path.string(), true))
			{
				Texture2D* t2d = Texture2D::create(file_path.filename().string());
				t2d->setImage(img);
				t2d->generate();
				return t2d;
			}

			return nullptr;
		}

		template<>
		static Shader* loadAndSave<>(const std::string& path)
		{
			Shader* shader = Shader::create(sRelativeResDir + "/" + path);
			shader->generate();
			shader->saveObject();

			mDataUSet.emplace(shader);
			return shader;
		}

		template<>
		static Shader* loadOnly<>(const std::string& path)
		{
			Shader* shader = Shader::create(sRelativeResDir + "/" + path);
			shader->generate();
			return shader;
		}

		template<>
		static Image* loadAndSave<>(const std::string& path)
		{
			Image* image = Image::create();
			if (image->openFile(sRelativeResDir + "/" + path, true))
			{
				image->saveObject();
				mDataUSet.emplace(image);
				return image;
			}

			return nullptr;
		}

		template<>
		static Image* loadOnly<>(const std::string& path)
		{
			Image* image = Image::create();
			if (image->openFile(sRelativeResDir + "/" + path, true))
			{
				return image;
			}

			return nullptr;
		}


		template<>
		static Model* loadAndSave<>(const std::string& path)
		{
			auto result = mModelMap.try_emplace(path, nullptr);
			if (result.second)
			{
				Model* model = Model::create();
				model->load(std::format("{}/{}", sRelativeResDir, path));
				model->saveObject();
				model->setPath(result.first->first);
				result.first->second = model;
			}

			return result.first->second;
		}

		template<>
		static Model* loadOnly<>(const std::string& path)
		{
			Model* model = Model::create();
			model->load(sRelativeResDir + "/" + path);
			return model;
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
			auto it = mDataUSet.begin();
			auto end = mDataUSet.end();
			while (it != mDataUSet.end())
			{
				if ((*it)->getRefCount() == 1)
				{
					(*it)->deleteObject();
					it = mDataUSet.erase(it);
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
		static std::unordered_map<std::string_view, Texture2D*> mTexture2DUSet;
		static std::unordered_map<std::string_view, Shader*> mShaderUSet;
		static std::unordered_map<std::string_view, Image*> mImageUSet;

		static std::unordered_map<std::string, Model*> mModelMap;

	private:
		static std::string sEngineDir;
		static std::string sResDir;

		static std::string sRelativeEngineDir;
		static std::string sRelativeResDir;
	};
}
