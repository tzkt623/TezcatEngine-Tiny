#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	struct TextureBufferInfo;
	class TextureRenderBuffer2D;
	class TextureBuffer2D;

	class TINY_API FrameBuffer
	{
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
		std::vector<TextureRenderBuffer2D*> mBuffers;

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
		static std::stack<FrameBuffer*> sFrameBufferStack;
		static FrameBuffer* sDefaultBuffer;
	};
}
