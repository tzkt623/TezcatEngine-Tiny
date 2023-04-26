#pragma once

#include "VertexConfig.h"
#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Head/Context.h"
#include "../Head/GLMHead.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	class MeshData;

	/// <summary>
	/// 对应gl的VBO
	/// </summary>
	class TINY_API VertexBuffer : public TinyObject
	{
		TINY_RTTI_H(VertexBuffer)
	public:
		VertexBuffer();
		virtual ~VertexBuffer() = 0;


		virtual void init(const void* data, const size_t& dataLenght) = 0;
		virtual void init(const size_t& dataLenght) = 0;
		virtual void bind() = 0;
		virtual void unbind() = 0;

		void setLayoutData(VertexPosition position, VertexLayoutType type);
		VertexLayoutData& getLayoutData() { return mLayoutData; }

		virtual void setData(const void* data, size_t size) = 0;

	protected:
		uint32_t mBufferID = 0;
		VertexLayoutData mLayoutData;
	};


	class TINY_API IndexBuffer : public TinyObject
	{
		TINY_RTTI_H(IndexBuffer)
	public:
		IndexBuffer();
		virtual ~IndexBuffer();

		virtual void init(const void* data, const size_t& dataLenght) = 0;
		virtual void bind() = 0;
		virtual void unbind() = 0;

	protected:
		uint32_t mBufferID = 0;
	};
}
