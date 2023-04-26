#pragma once
#include "GLHead.h"

#include "Core/Renderer/FrameBuffer.h"
#include "Core/Manager/FrameBufferManager.h"

using namespace tezcat::Tiny;
namespace tezcat::Tiny::GL
{
	class TINY_API GLFrameBufferDefault : public FrameBuffer
	{
		GLFrameBufferDefault();
		TINY_Factory(GLFrameBufferDefault)
		TINY_RTTI_H(GLFrameBufferDefault)
	public:
		virtual ~GLFrameBufferDefault();


		void attach(TextureRenderBuffer2D* buffer) final;
		void build(const std::function<void(FrameBuffer*) >& function) final;

	protected:
		void bind() final;
		//void unbind() final;
	};

	class TINY_API GLFrameBuffer : public FrameBuffer
	{
		GLFrameBuffer();
		TINY_Factory(GLFrameBuffer)
		TINY_RTTI_H(GLFrameBuffer)
	public:
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
