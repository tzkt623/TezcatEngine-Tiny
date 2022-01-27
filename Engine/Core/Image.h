#pragma once

#include <string>

namespace tezcat::Tiny::Core
{
	class Image
	{
	public:
		Image();
		~Image();

		void openFile(const std::string& path);
		int getWidth() const { return m_Width; }
		int getHeight() const { return m_Height; }
		unsigned char* getData() { return m_Data; }

	private:
		int m_Width;
		int m_Height;
		int m_Channels;
		unsigned char* m_Data;
	};
}

