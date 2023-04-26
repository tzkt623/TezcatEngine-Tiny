#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "ThirdParty/stb_image.h"

namespace tezcat::Tiny
{
	Image::Image()
		: mWidth(0)
		, mHeight(0)
		, mChannels(0)
		, mData(nullptr)
		, mIsHDR(false)
	{

	}

	Image::Image(Image&& other) noexcept
		: mWidth(other.mWidth)
		, mHeight(other.mWidth)
		, mChannels(other.mWidth)
		, mData(other.mData)
		, mIsHDR(other.mIsHDR)
	{
		other.mData = nullptr;
	}

	Image::~Image()
	{
		stbi_image_free(mData);
	}

	void Image::openFile(const std::string& path, bool flip)
	{
		stbi_set_flip_vertically_on_load(flip);
		mData = stbi_load(path.c_str(), &mWidth, &mHeight, &mChannels, 0);
		if (mData == nullptr)
		{
			std::cout << "Image Load Error: " << path << std::endl;
		}
	}

	void Image::openFile(const FileInfo& info, bool flip /*= false*/)
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
	}
}
