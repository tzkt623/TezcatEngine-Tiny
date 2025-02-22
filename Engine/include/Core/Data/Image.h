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
