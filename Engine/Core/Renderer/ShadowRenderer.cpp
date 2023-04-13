#include "ShadowRenderer.h"
#include "RenderPass.h"
#include "BaseGraphics.h"
#include "FrameBuffer.h"

#include "../Component/Light.h"
#include "../Component/Transform.h"

#include "../Manager/FrameBufferManager.h"
#include "../Renderer/Texture.h"



namespace tezcat::Tiny::Core
{
	RenderPass* ShadowRenderer::mPass = nullptr;
	FrameBuffer* ShadowRenderer::mFrameBuffer = nullptr;
	ViewportInfo ShadowRenderer::mViewInfo;

	void ShadowRenderer::init(int x, int y, int width, int height)
	{
		mViewInfo.OX = x;
		mViewInfo.OY = y;
		mViewInfo.Width = width;
		mViewInfo.Height = height;

		//shadow pass
		mPass = RenderPass::get("Shadow");
		//shadow framebuffer
		mFrameBuffer = FrameBufferMgr::getInstance()->create("Shadow",
			mViewInfo.Width, mViewInfo.Height,
			{
				//shadow texture
				TextureBufferInfo("Shadow"
					, TextureBufferType::DepthComponent
					, TextureChannel::Depth
					, TextureChannel::Depth
					, DataType::Float32
					, true)
			});
	}

	void ShadowRenderer::beginRender()
	{
		mFrameBuffer->bind();
	}

	void ShadowRenderer::endRender()
	{
		mFrameBuffer->unbind();
	}

	void ShadowRenderer::render(BaseGraphics* graphics, ILight* light)
	{
		if (mPass->checkState())
		{
			graphics->clear(ClearOption::CO_Depth);
			graphics->setViewport(mViewInfo);

			auto shader = mPass->getShader();
			auto lm4 = glm::ortho(mViewInfo.OX, mViewInfo.Width, mViewInfo.OX, mViewInfo.Height);
			shader->setProjectionMatrix(lm4);

			auto com = dynamic_cast<DirectionalLight*>(light);
			glm::mat4 lightView = glm::lookAt(com->getTransform()->getPosition()
				, com->getDirection()
				, com->getTransform()->getUp());

			shader->setModelMatrix(com->getTransform()->getModelMatrix());
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
