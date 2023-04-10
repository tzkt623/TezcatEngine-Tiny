#pragma once
#include "GLHead.h"

#include "Core/Renderer/FrameBuffer.h"
#include "Core/Manager/FrameBufferManager.h"

using namespace tezcat::Tiny::Core;
namespace tezcat::Tiny::GL
{
	class TINY_API GLFrameBuffer : public FrameBuffer
	{
	public:
		GLFrameBuffer();
		~GLFrameBuffer();

		void bind() override;
		void unbind() override;

		void attach(TextureRenderBuffer2D* buffer) override;
		void build(const std::function<void(FrameBuffer*) >& function) override;

	private:
		void createTextureBuffer(TextureBuffer2D* tex);
		void createRenderBuffer(TextureRenderBuffer2D* tex);

	private:
		uint32_t mColorCount;
	};

	//-------------------------------------------------
	//
	//
	//

	class TINY_API GLFrameBufferCreator : public FrameBufferCreator
	{
	protected:
		FrameBuffer* createFrameBuffer() override;
	};
}
