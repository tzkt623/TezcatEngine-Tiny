#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class TINY_API Image
	{
	public:
		Image();
		Image(Image&& other);
		~Image();

		void openFile(const std::string& path, bool flip = false);
		inline const int& getWidth() const { return mWidth; }
		inline const int& getHeight() const { return mHeight; }
		inline const int& getChannels() const { return mChannels; }
		inline unsigned char* getData() const { return mData; }

		Image& operator=(Image&& other) noexcept
		{
			mWidth = other.mWidth;
			mHeight = other.mHeight;
			mChannels = other.mChannels;
			mData = other.mData;
			other.mData = nullptr;
			return *this;
		}

	private:
		int mWidth;
		int mHeight;
		int mChannels;
		unsigned char* mData;
	};
}
