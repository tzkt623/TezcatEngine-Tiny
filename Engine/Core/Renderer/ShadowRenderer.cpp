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
		mPass = RenderPass::get("ShadowMap");
		//shadow framebuffer
		mFrameBuffer = FrameBufferMgr::getInstance()->create("Shadow",
			mViewInfo.Width, mViewInfo.Height,
			{
				//shadow texture
				TextureBufferInfo("Shadow"
					, TextureBufferType::DepthComponent
					, TextureFilter::Nearest
					, TextureWrap::Repeat
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
			graphics->setViewport(mViewInfo);
			graphics->clear(ClearOption::CO_Depth);

			const float near = 0.1f;
			const float far = 2000.0f;

			auto shader = mPass->getShader();

			float half_w = mViewInfo.Width / 2.0f;
			float half_h = mViewInfo.Width / 2.0f;
			auto light_projection = glm::ortho(
				-half_w, half_w,
				-half_h, half_h,
				near, far);

			auto lit_dir = (DirectionalLight*)(light);
			auto lit_transform = lit_dir->getTransform();
			glm::mat4 lightView = glm::lookAt(
				lit_transform->getWorldPosition(),
				lit_transform->getWorldPosition() + lit_transform->getForward(),
				lit_transform->getUp());

			auto lpv = light_projection * lightView;
			shader->setMat4(ShaderParam::MatrixLit, lpv);

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
