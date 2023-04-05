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

		void create(TextureRenderBuffer2D* targetBuffer2D) override;
		void create(const int& width, const int& high, const std::initializer_list<TextureBufferInfo>& infos) override;

	private:
		void createTextureBuffer(const int& width, const int& high, const TextureBufferInfo& info, int& colorCountor);
		void createRenderBuffer(const int& width, const int& high, const TextureBufferInfo& info);

	};

	//-------------------------------------------------
	//
	//
	//

	class TINY_API GLFrameBufferCreator : public FrameBufferCreator
	{
	public:
		FrameBuffer* create(const int& width, const int& high, const std::initializer_list<TextureBufferInfo>& infos) override;
	};
}
