#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Tool/FileTool.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	class TINY_API Image : public TinyObject
	{
		Image();
		TINY_OBJECT_H(Image, TinyObject)
	public:
		virtual ~Image();

		bool openFile(const std::string& path, bool flip = false);
		bool openFile(const FileInfo& info, bool flip = false);

		const int& getWidth() const { return mWidth; }
		const int& getHeight() const { return mHeight; }
		const int& getChannels() const { return mChannels; }
		uint64 getDataSize() const
		{
			return mWidth * mHeight * mChannels * uint64(mIsHDR ? sizeof(float) : sizeof(uint8));
		}
		void* getData() const { return mData; }
		void* moveData()
		{
			auto temp = mData;
			mData = nullptr;
			return temp;
		}
		bool isHDR() const { return mIsHDR; }

	private:
		int mWidth;
		int mHeight;
		int mChannels;
		void* mData;
		bool mIsHDR;
	};
}
