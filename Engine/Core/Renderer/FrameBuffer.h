#pragma once

#include "../Head/TinyCpp.h"
#include "../Head/ConfigHead.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	struct TextureInfo;
	class Texture;
	class Texture2D;
	class TextureCube;
	class TextureRender2D;
	class BaseGraphics;

	class TINY_API FrameBuffer : public TinyObject
	{
		FrameBuffer();
		FrameBuffer(const std::string& name);
		TINY_RTTI_H(FrameBuffer);
		TINY_Factory(FrameBuffer);
	public:
		virtual ~FrameBuffer();
		void generate();


		void apply(uint32_t id) { mBufferID = id; }
		uint32_t getFrameBufferID() { return mBufferID; }
		TinyVector<Texture*>& getAttachmentes() { return mComponents; }
		void addAttachment(Texture* tex);
		Texture* getBuffer(const int& index);

	protected:
		std::string mName;
		uint32_t mBufferID;
		TinyVector<Texture*> mComponents;

	public:
		static void bind(BaseGraphics* graphics, FrameBuffer* buffer);
		static void unbind(BaseGraphics* graphics, FrameBuffer* buffer);
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

