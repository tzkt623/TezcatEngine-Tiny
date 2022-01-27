#include "Image.h"

#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


namespace tezcat::Tiny::Core
{
	Image::Image() :
		m_Width(0),
		m_Height(0),
		m_Channels(0),
		m_Data(nullptr)
	{

	}

	Image::~Image()
	{
		stbi_image_free(m_Data);
	}

	void Image::openFile(const std::string& path)
	{
		stbi_set_flip_vertically_on_load(true);
		m_Data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Channels, 0);
		if (m_Data == nullptr)
		{
			std::cout << "Í¼Æ¬¼ÓÔØÊ§°Ü: " << path << std::endl;
		}
	}
}
