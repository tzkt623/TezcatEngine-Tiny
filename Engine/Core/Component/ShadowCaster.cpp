#include "ShadowCaster.h"
#include "Transform.h"

#include "../Shader/ShaderParam.h"
#include "../Shader/Shader.h"

#include "../Manager/FrameBufferManager.h"
#include "../Renderer/Texture.h"
#include "../Renderer/FrameBuffer.h"
#include "../Renderer/RenderPass.h"

#include "../Manager/ShadowCasterManager.h"

namespace tezcat::Tiny
{
	TINY_RTTI_CPP(ShadowCaster);
	ShadowCaster::ShadowCaster()
		: IRenderObserver(new ExtraQueue(this))
		, mCasterID(0)
	{
		mClearOption = ClearOption::CO_Depth;
		mRenderPhase = RenderPhase::Shadow;
	}

	ShadowCaster::~ShadowCaster()
	{

	}

	void ShadowCaster::submitViewMatrix(Shader* shader)
	{
		this->updateObserverMatrix();

		auto transform = this->getTransform();
		mViewMatrix = glm::lookAt(transform->getWorldPosition()
								, transform->getWorldPosition() + transform->getForward()
								, transform->getUp());

		auto lpv = mProjectionMatrix * mViewMatrix;
		shader->setMat4(ShaderParam::MatrixLit, lpv);
	}

	void ShadowCaster::submit(Shader* shader)
	{

	}

	void ShadowCaster::onStart()
	{

	}

	void ShadowCaster::onEnable()
	{
		mCasterID = ShadowCasterMgr::getInstance()->addCaster(this);
	}

	void ShadowCaster::onDisable()
	{
		ShadowCasterMgr::getInstance()->removeCaster(this);
	}

	void ShadowCaster::onDestroy()
	{
		mFrameBuffer->subRef();
	}

	void ShadowCaster::setShadowMap(int width, int height, const std::string& shaderName)
	{
		if (mFrameBuffer)
		{
			mFrameBuffer->subRef();
		}

		mViewInfo.OX = 0;
		mViewInfo.OY = 0;
		mViewInfo.Width = width;
		mViewInfo.Height = height;

		mFrameBuffer = FrameBufferMgr::getInstance()->create(
			"Shadow",
			width, height,
			{
				//shadow texture
				TextureInfo("Shadow"
						  , TextureType::Texture2D
						  , TextureAttachPosition::DepthComponent
						  , TextureFilter::Nearest
						  , TextureFilter::Nearest
						  , TextureWrap::Clamp_To_Border
						  , TextureWrap::Clamp_To_Border
						  , TextureChannel::Depth
						  , TextureChannel::Depth
						  , DataType::Float32)
			});

		mFrameBuffer->addRef();
	}
}


