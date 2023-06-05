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
		TINY_Factory(Image);
		TINY_RTTI_H(Image);
	public:
		virtual ~Image();

		bool openFile(const std::string& path, bool flip = false);
		bool openFile(const FileInfo& info, bool flip = false);

		inline const int& getWidth() const { return mWidth; }
		inline const int& getHeight() const { return mHeight; }
		inline const int& getChannels() const { return mChannels; }
		uint64_t getDataSize() const
		{
			return mWidth * mHeight * mChannels * uint64_t(mIsHDR ? sizeof(float) : sizeof(uint8_t));
		}
		inline void* getData() const { return mData; }
		inline void* moveData()
		{
			auto temp = mData;
			mData = nullptr;
			return temp;
		}
		inline bool isHDR() const { return mIsHDR; }

	private:
		int mWidth;
		int mHeight;
		int mChannels;
		void* mData;
		bool mIsHDR;
	};
}
