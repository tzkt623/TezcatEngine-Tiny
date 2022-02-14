#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class TINY_API Image
	{
	public:
		Image();
		~Image();

		void openFile(const std::string& path);
		inline int getWidth() const { return m_Width; }
		inline int getHeight() const { return m_Height; }
		inline int getChannels() const { return m_Channels; }
		inline unsigned char* getData() { return m_Data; }

	private:
		int m_Width;
		int m_Height;
		int m_Channels;
		unsigned char* m_Data;
	};
}

