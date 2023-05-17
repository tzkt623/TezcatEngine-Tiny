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
		TINY_Factory(GLFrameBufferDefault);
		TINY_RTTI_H(GLFrameBufferDefault);
	public:
		virtual ~GLFrameBufferDefault();

		void beginBuild() override {}
		void endBuild() override {}
		void attach(Texture* tex) final {}

	public:
		void attach2D(Texture2D* tex) final {}
		void attachRender(TextureRender2D* tex) final {}
		void attachCube(TextureCube* tex) final {}
		void attachCube(TextureCube* tex, int colorIndex, int faceIndex) {};

	protected:
		void bind() final;
		void unbind() final;
	};

	class TINY_API GLFrameBuffer : public FrameBuffer
	{
		GLFrameBuffer();
		TINY_Factory(GLFrameBuffer);
		TINY_RTTI_H(GLFrameBuffer);
	public:
		~GLFrameBuffer();

		void beginBuild() override;
		void endBuild() override;
		void attach(Texture* tex) override;

	public:
		void attach2D(Texture2D* tex) override;
		void attachRender(TextureRender2D* tex) override;
		void attachCube(TextureCube* tex) override;
		void attachCube(TextureCube* tex, int colorIndex, int faceIndex) override;

	protected:
		void bind() override;
		void unbind() override;


	private:
		uint32_t mColorCount : 8;
		bool mBuild;
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
