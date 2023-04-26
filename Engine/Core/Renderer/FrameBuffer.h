#pragma once

#include "../Head/TinyCpp.h"
#include "../Head/ConfigHead.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	struct TextureBufferInfo;
	class TextureRenderBuffer2D;
	class TextureBuffer2D;

	class TINY_API FrameBuffer : public TinyObject
	{
		TINY_RTTI_H(FrameBuffer)
	public:
		FrameBuffer();
		virtual ~FrameBuffer();
		virtual void attach(TextureRenderBuffer2D* buffer) = 0;
		virtual void build(const std::function<void(FrameBuffer*)>& function) = 0;

		TextureRenderBuffer2D* getBuffer(const int& index);

	protected:
		virtual void bind() = 0;
		//virtual void unbind() = 0;

	protected:
		uint32_t mBufferID;
		TinyVector<TextureRenderBuffer2D*> mBuffers;

	public:
		static void bind(FrameBuffer* buffer);
		static void unbind(FrameBuffer* buffer);
		static FrameBuffer* getDefaultBuffer() { return sDefaultBuffer; }
		static void setDefaultBuffer(FrameBuffer* buffer)
		{
			sDefaultBuffer = buffer;
			sFrameBufferStack.push(sDefaultBuffer);
		}

	private:
		static TinyStack<FrameBuffer*> sFrameBufferStack;
		static FrameBuffer* sDefaultBuffer;
	};
}
