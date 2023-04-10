#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "RenderConfig.h"

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

		virtual void bind() = 0;
		virtual void unbind() = 0;
		virtual void attach(TextureRenderBuffer2D* buffer) = 0;
		virtual void build(const std::function<void(FrameBuffer*)>& function) = 0;

		TextureRenderBuffer2D* getBuffer(const int& index);

		void setViewport(const int& ox, const int& oy, const int& width, const int& height)
		{
			mViewportInfo.OX = ox;
			mViewportInfo.OY = oy;
			mViewportInfo.Width = width;
			mViewportInfo.Height = height;
		}

		void setViewport(const int& width, const int& height)
		{
			mViewportInfo.Width = width;
			mViewportInfo.Height = height;
		}

		void setControlRenderSize(bool val)
		{
			mControlRenderSize = val;
		}

	protected:
		bool mControlRenderSize;
		uint32_t mBufferID;
		ViewportInfo mViewportInfo;
		std::vector<TextureRenderBuffer2D*> mBuffers;
	};
}
