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

		void openFile(const std::string& path, bool flip = true);
		inline int getWidth() const { return mWidth; }
		inline int getHeight() const { return mHeight; }
		inline int getChannels() const { return mChannels; }
		inline unsigned char* getData() { return mData; }

	private:
		int mWidth;
		int mHeight;
		int mChannels;
		unsigned char* mData;
	};
}
