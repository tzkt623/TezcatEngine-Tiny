#include "ShadowRenderer.h"
#include "RenderPass.h"
#include "BaseGraphics.h"
#include "FrameBuffer.h"

#include "../Component/Light.h"
#include "../Component/Transform.h"

#include "../Manager/FrameBufferManager.h"
#include "../Renderer/Texture.h"



namespace tezcat::Tiny
{
	RenderPass* ShadowRenderer::mPass = nullptr;
	FrameBuffer* ShadowRenderer::mFrameBuffer;
	ViewportInfo ShadowRenderer::mViewInfo;

	void ShadowRenderer::init(int x, int y, int width, int height)
	{
		mViewInfo.OX = x;
		mViewInfo.OY = y;
		mViewInfo.Width = width;
		mViewInfo.Height = height;

		//shadow pass
		mPass = RenderPass::get("ShadowMap");
		//shadow framebuffer
		mFrameBuffer = FrameBufferMgr::getInstance()->create("Shadow",
			width, height,
			{
				//shadow texture
				TextureBufferInfo("Shadow"
					, TextureBufferType::DepthComponent
					, TextureFilter::Nearest
					, TextureWrap::Clamp_To_Border
					, TextureChannel::Depth
					, TextureChannel::Depth
					, DataType::Float32
					, false
					, true)
			});
	}

	void ShadowRenderer::beginRender()
	{
		FrameBuffer::bind(mFrameBuffer);
	}

	void ShadowRenderer::endRender()
	{
		FrameBuffer::unbind(mFrameBuffer);
	}

	void ShadowRenderer::render(BaseGraphics* graphics, ILight* light)
	{
		if (mPass->checkState())
		{
			auto shader = mPass->getShader();

			graphics->setViewport(mViewInfo);
			graphics->clear(ClearOption::CO_Depth);

			light->submitViewMatrix(shader);
			mPass->renderMeshOnly(graphics);
		}
	}

	void ShadowRenderer::setPass(const std::string& name)
	{
		mPass = RenderPass::get(name);
	}

	void ShadowRenderer::addMesh(IRenderMesh* mesh)
	{
		mPass->addRenderMesh(mesh);
	}


}
