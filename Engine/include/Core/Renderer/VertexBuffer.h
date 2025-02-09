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

#include "VertexConfig.h"
#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Head/RenderConfig.h"
#include "../Head/GLMHead.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	class TINY_API IBuffer : public TinyObject
	{
		TINY_ABSTRACT_OBJECT_H(IBuffer, TinyObject)

	protected:
		IBuffer();

	protected:
		virtual void init() override { }

	public:
		virtual ~IBuffer();

		void init(const size_t& dataSize, const void* data = nullptr)
		{
			auto temp = realloc(mData, dataSize);
			if (temp)
			{
				mDataSize = dataSize;
				mData = temp;
				memcpy_s(mData, dataSize, data, dataSize);
			}
		}

		void updateData(const void* data, const size_t& dataSize)
		{
			auto temp = realloc(mData, dataSize);
			if (temp)
			{
				mDataSize = dataSize;
				mData = temp;
				memcpy_s(mData, dataSize, data, dataSize);
			}
		}

		void apply(uint32 id) { mBufferID = id; }
		const uint32 &getBufferID() const { return mBufferID; }

		const void* getData() const
		{
			return mData;
		};

		uint64 getDataSize() const
		{
			return mDataSize;
		};

		void apply()
		{
			free(mData);
		}

	protected:
		uint32 mBufferID = 0;
		void* mData;
		uint64 mDataSize;
	};

	/// <summary>
	/// 对应gl的VBO
	/// </summary>
	class TINY_API VertexBuffer : public IBuffer
	{
		VertexBuffer();
		TINY_OBJECT_H(VertexBuffer, IBuffer)

	public:
		virtual ~VertexBuffer();


		void setLayoutData(VertexPosition position, VertexLayoutType type);
		const VertexLayoutData& getLayoutData() const { return mLayoutData; }

	protected:
		VertexLayoutData mLayoutData;
	};


	class TINY_API IndexBuffer : public IBuffer
	{
		IndexBuffer();
		TINY_OBJECT_H(IndexBuffer, IBuffer)

	public:
		virtual ~IndexBuffer();
	};
}
