#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Tool/FileTool.h"

namespace tezcat::Tiny
{
	class TINY_API Image
	{
	public:
		Image();
		Image(Image&& other) noexcept;
		~Image();

		void openFile(const std::string& path, bool flip = false);
		void openFile(const FileInfo& info, bool flip = false);

		inline const int& getWidth() const { return mWidth; }
		inline const int& getHeight() const { return mHeight; }
		inline const int& getChannels() const { return mChannels; }
		inline void* getData() const { return mData; }
		inline bool isHDR() const { return mIsHDR; }

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
		void* mData;
		bool mIsHDR;
	};
}
