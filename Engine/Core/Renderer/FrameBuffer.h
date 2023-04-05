#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	struct TextureBufferInfo;
	class TextureRenderBuffer2D;
	class TINY_API FrameBuffer
	{
	public:
		FrameBuffer();
		virtual ~FrameBuffer();

		virtual void bind() = 0;
		virtual void unbind() = 0;
		virtual void create(TextureRenderBuffer2D* targetBuffer2D) = 0;
		virtual void create(const int& width, const int& high, const std::initializer_list<TextureBufferInfo>& infos) = 0;

		TextureRenderBuffer2D* getBuffer(const int& index);

	protected:
		unsigned int mBufferID;
		std::vector<TextureRenderBuffer2D*> mBuffers;
	};
}
