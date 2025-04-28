#pragma once
/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


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

		bool openFile(const file_path& path, bool flip = true);
		bool openFile(const FileInfo& info, bool flip = true);

		const int32_t& getWidth() const { return mWidth; }
		const int32_t& getHeight() const { return mHeight; }
		const int32_t& getChannels() const { return mChannels; }
		uint64_t getDataSize() const
		{
			return mWidth * mHeight * mChannels * uint64_t(mIsHDR ? sizeof(float) : sizeof(uint8_t));
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
		void loadWithSTBI(const file_path& path, bool flip);
		void loadWithTinyExr(const file_path& path, bool flip);


	private:
		int32_t mWidth;
		int32_t mHeight;
		int32_t mChannels;
		void* mData;
		bool mIsHDR;
	};
}
