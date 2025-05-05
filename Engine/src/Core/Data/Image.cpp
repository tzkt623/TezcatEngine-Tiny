/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

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

#include "Core/Data/Image.h"
#include "Core/Debug/Debug.h"

#define STBI_WINDOWS_UTF8
#define STB_IMAGE_IMPLEMENTATION
#include "ThirdParty/ImageLoad/stb_image.h"

#define TINYEXR_USE_STB_ZLIB 1
#define TINYEXR_USE_PIZ 1
//#define TINYEXR_USE_ZFP 1
#define TINYEXR_USE_THREAD 1
#define TINYEXR_IMPLEMENTATION
#include "ThirdParty/ImageLoad/tinyexr.h"

namespace tezcat::Tiny
{
	struct TINY_API StbiHelper
	{
		std::ifstream file;

		~StbiHelper()
		{
			file.close();
		}

		static int read(void* user, char* data, int size)
		{
			StbiHelper* helper = (StbiHelper*)user;
			helper->file.read(data, size);
			return size;
		}

		static void skip(void* user, int n)
		{
			StbiHelper* helper = (StbiHelper*)user;
			helper->file.seekg(n, std::ios::cur);
		}

		static int eof(void* user)
		{
			StbiHelper* helper = (StbiHelper*)user;
			return (int)helper->file.eof();
		}
	};

	TINY_OBJECT_CPP(Image, TinyObject);
	Image::Image()
		: mWidth(0)
		, mHeight(0)
		, mChannels(0)
		, mData(nullptr)
		, mIsHDR(false)
	{

	}

	Image::~Image()
	{
		stbi_image_free(mData);
		mData = nullptr;
	}

	void Image::loadWithSTBI(const file_path& path, bool flip)
	{
		std::unique_ptr<StbiHelper> helper = std::make_unique<StbiHelper>();
		helper->file.open(path, std::ios::binary);
		if (!helper->file.is_open())
		{
			return;
		}

		stbi_io_callbacks callbacks
		{
			.read = StbiHelper::read,
			.skip = StbiHelper::skip,
			.eof = StbiHelper::eof
		};

		stbi_set_flip_vertically_on_load(flip);
		if (stbi_is_hdr_from_callbacks(&callbacks, (void*)helper.get()) > 0)
		{
			helper->file.seekg(0);
			mIsHDR = true;
			mData = stbi_loadf_from_callbacks(&callbacks, (void*)helper.get(), &mWidth, &mHeight, &mChannels, 0);
		}
		else
		{
			helper->file.seekg(0);
			mData = stbi_load_from_callbacks(&callbacks, (void*)helper.get(), &mWidth, &mHeight, &mChannels, 0);
		}
	}

	bool Image::openFile(const file_path& path, bool flip)
	{
		file_path true_path = file_sys_helper::generic(path);

		if (true_path.extension() == ".exr")
		{
			this->loadWithTinyExr(true_path, flip);
		}
		else
		{
			this->loadWithSTBI(true_path, flip);
		}

		return mData != nullptr;
	}

	bool Image::openFile(const FileInfo& info, bool flip /*= false*/)
	{
		stbi_set_flip_vertically_on_load(flip);
		FILE* f = stbi__fopen(info.path.string().c_str(), "rb");
		if (!f)
		{
			return stbi__errpuc("can't fopen", "Unable to open file");
		}

		if (stbi_is_hdr_from_file(f) > 0)
		{
			mIsHDR = true;
			mData = stbi_loadf_from_file(f, &mWidth, &mHeight, &mChannels, 0);
		}
		else
		{
			mData = stbi_load_from_file(f, &mWidth, &mHeight, &mChannels, 0);
		}

		fclose(f);
		return mData != nullptr;
	}

	void Image::loadWithTinyExr(const file_path& path, bool flip)
	{
		float* out = nullptr;
		const char* err = nullptr;
		int ret = LoadEXR(&out, &mWidth, &mHeight, path.string().c_str(), &err);

		if (ret != TINYEXR_SUCCESS)
		{
			if (err)
			{
				TINY_LOG_WARNING(err);
				FreeEXRErrorMessage(err); // release memory of error message.
			}
		}
		else
		{
			free(out); // release memory of image data
		}
	}

}
