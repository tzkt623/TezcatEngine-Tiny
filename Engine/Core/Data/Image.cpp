#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "ThirdParty/stb_image.h"

namespace tezcat::Tiny
{
	TINY_RTTI_CPP(Image);
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
	}

	bool Image::openFile(const std::string& path, bool flip)
	{
		stbi_set_flip_vertically_on_load(flip);
		std::filesystem::path full_path(path);
		auto ext = full_path.extension().string();
		if (ext == ".hdr")
		{
			mData = stbi_loadf(path.c_str(), &mWidth, &mHeight, &mChannels, 0);
			mIsHDR = true;
		}
		else
		{
			mData = stbi_load(path.c_str(), &mWidth, &mHeight, &mChannels, 0);
		}

		return mData != nullptr;
	}

	bool Image::openFile(const FileInfo& info, bool flip /*= false*/)
	{
		stbi_set_flip_vertically_on_load(flip);
		if (info.type == FileType::FT_Hdr)
		{
			mData = stbi_loadf(info.path.c_str(), &mWidth, &mHeight, &mChannels, 0);
			mIsHDR = true;
		}
		else
		{
			mData = stbi_load(info.path.c_str(), &mWidth, &mHeight, &mChannels, 0);
		}

		return mData != nullptr;
	}
}
