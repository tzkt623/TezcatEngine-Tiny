#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "ThirdParty/stb_image.h"

namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(Image, TinyObject)
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

	bool Image::openFile(const std::string& path, bool flip)
	{
		stbi_set_flip_vertically_on_load(flip);
		FILE* f = stbi__fopen(path.c_str(), "rb");
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

	bool Image::openFile(const FileInfo& info, bool flip /*= false*/)
	{
		stbi_set_flip_vertically_on_load(flip);
		FILE* f = stbi__fopen(info.path.c_str(), "rb");
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
}
