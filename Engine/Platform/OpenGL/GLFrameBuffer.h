#pragma once
#include "GLHead.h"

#include "Core/Renderer/FrameBuffer.h"
#include "Core/Manager/FrameBufferManager.h"

using namespace tezcat::Tiny::Core;
namespace tezcat::Tiny::GL
{
	class TINY_API GLFrameBufferDefault : public FrameBuffer
	{
	public:
		GLFrameBufferDefault();
		virtual ~GLFrameBufferDefault();


		void attach(TextureRenderBuffer2D* buffer) final;
		void build(const std::function<void(FrameBuffer*) >& function) final;

	protected:
		void bind() final;
		//void unbind() final;
	};

	class TINY_API GLFrameBuffer : public FrameBuffer
	{
	public:
		GLFrameBuffer();
		~GLFrameBuffer();

		void attach(TextureRenderBuffer2D* buffer) override;
		void build(const std::function<void(FrameBuffer*) >& function) override;

	protected:
		void bind() override;
		//void unbind() override;

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
	public:
		GLFrameBufferCreator();

	protected:
		FrameBuffer* createFrameBuffer() override;
	};
}
