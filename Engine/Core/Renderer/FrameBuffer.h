#pragma once

#include "../Head/TinyCpp.h"
#include "../Head/ConfigHead.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	struct TextureInfo;
	class Texture;
	class Texture2D;
	class TextureRender2D;
	class TextureCube;

	class TINY_API FrameBuffer : public TinyObject
	{
		TINY_RTTI_H(FrameBuffer);
	public:
		FrameBuffer();
		virtual ~FrameBuffer();

		virtual void beginBuild() = 0;
		virtual void endBuild() = 0;
		virtual void attach(Texture* buffer) = 0;

	public:
		virtual void attach2D(Texture2D* tex) = 0;
		virtual void attachRender(TextureRender2D* tex) = 0;
		virtual void attachCube(TextureCube* tex) = 0;
		virtual void attachCube(TextureCube* tex, int colorIndex, int faceIndex) = 0;

		Texture* getBuffer(const int& index);

	protected:
		virtual void bind() = 0;
		virtual void unbind() = 0;

	protected:
		uint32_t mBufferID;
		TinyVector<Texture*> mBuffers;

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

