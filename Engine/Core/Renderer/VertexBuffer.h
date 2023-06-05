#pragma once

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
	public:
		IBuffer()
			: mBufferID(0)
			, mData(nullptr)
			, mDataSize(0)
		{}

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

		void apply(uint32_t id) { mBufferID = id; }
		uint32_t getBufferID() { return mBufferID; }

		const void* getData()
		{
			return mData;
		};

		uint64_t getDataSize()
		{
			return mDataSize;
		};

		void apply()
		{
			free(mData);
		}

	protected:
		uint32_t mBufferID = 0;
		void* mData;
		size_t mDataSize;
	};

	/// <summary>
	/// 对应gl的VBO
	/// </summary>
	class TINY_API VertexBuffer : public IBuffer
	{
		VertexBuffer();
		TINY_RTTI_H(VertexBuffer);
		TINY_Factory(VertexBuffer);

	public:
		virtual ~VertexBuffer();


		void setLayoutData(VertexPosition position, VertexLayoutType type);
		VertexLayoutData& getLayoutData() { return mLayoutData; }

	protected:
		VertexLayoutData mLayoutData;
	};


	class TINY_API IndexBuffer : public IBuffer
	{
		IndexBuffer();
		TINY_RTTI_H(IndexBuffer);
		TINY_Factory(IndexBuffer);

	public:
		virtual ~IndexBuffer();
	};
}
