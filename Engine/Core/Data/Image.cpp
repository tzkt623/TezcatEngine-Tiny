#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "ThirdParty/stb_image.h"


namespace tezcat::Tiny::Core
{
	Image::Image()
		: mWidth(0)
		, mHeight(0)
		, mChannels(0)
		, mData(nullptr)
	{

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
}
