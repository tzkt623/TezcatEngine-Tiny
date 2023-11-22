#pragma once
#include "Image.h"

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

	class Resource
	{
		Resource() = delete;
		~Resource() = delete;
	public:
		static void init(const std::string& folderPath);
		static const std::string& getEngineDir() { return sEngineDir; }
		static const std::string& getResDir() { return sResDir; }

		static const std::string& getRelativeEngineDir() { return sRelativeEngineDir; }
		static const std::string& getRelativeResDir() { return sRelativeResDir; }

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

		template<>
		static Texture2D* load<>(const std::string& path)
		{
			std::filesystem::path file_path(sRelativeResDir + "/" + path);
			Image* img = Image::create();
			if (img->openFile(file_path.string(), true))
			{
				Texture2D* t2d = Texture2D::create(file_path.filename().string());
				t2d->setData(img);
				t2d->generate();
				t2d->addRef();

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
				t2d->setData(img);
				t2d->generate();
				return t2d;
			}

			return nullptr;
		}

		template<>
		static Shader* load<>(const std::string& path)
		{
			Shader* shader = Shader::create(sRelativeResDir + "/" + path);
			shader->generate();
			shader->addRef();

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
		static Image* load<>(const std::string& path)
		{
			Image* image = Image::create();
			if (image->openFile(sRelativeResDir + "/" + path, true))
			{
				image->addRef();
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
		static Model* load<>(const std::string& path)
		{
			Model* model = Model::create();
			model->load(sRelativeResDir + "/" + path);
			model->addRef();
			mDataUSet.emplace(model);
			return model;
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

		template<>
		static void unload<>(Model* data)
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
				if ((*it)->getRefCount() == 1)
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
#pragma endregion

	private:
		static std::unordered_set<TinyObject*> mDataUSet;
		static std::unordered_map<std::string, Texture2D*> mTexture2DUSet;
		static std::unordered_map<std::string, Shader*> mShaderUSet;
		static std::unordered_map<std::string, Image*> mImageUSet;

	private:
		static std::string sEngineDir;
		static std::string sResDir;

		static std::string sRelativeEngineDir;
		static std::string sRelativeResDir;
	};
}
