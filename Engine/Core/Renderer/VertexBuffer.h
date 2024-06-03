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
		uint32 getBufferID() const { return mBufferID; }

		const void* getData()
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
