#include "ShadowCaster.h"
#include "Transform.h"

#include "../Shader/ShaderParam.h"
#include "../Shader/Shader.h"

#include "../Manager/FrameBufferManager.h"
#include "../Manager/TextureManager.h"
#include "../Renderer/Texture.h"
#include "../Renderer/FrameBuffer.h"
#include "../Renderer/BaseGraphics.h"
#include "../Renderer/RenderPass.h"

#include "../Manager/ShadowCasterManager.h"

namespace tezcat::Tiny
{
	TINY_RTTI_CPP(ShadowCaster);
	ShadowCaster::ShadowCaster()
		: IRenderObserver(new ExtraQueue(this))
		, mCasterID(0)
		, mShadwowTexutre(nullptr)
	{
		mClearOption = ClearOption::CO_Depth;
		mRenderPhase = RenderPhase::Shadow;
	}

	ShadowCaster::~ShadowCaster()
	{

	}

	void ShadowCaster::submitViewMatrix(BaseGraphics* graphcis, Shader* shader)
	{
		this->updateObserverMatrix();

		auto transform = this->getTransform();
		mViewMatrix = glm::lookAt(transform->getWorldPosition()
								, transform->getWorldPosition() + transform->getForward()
								, transform->getUp());

		auto lpv = mProjectionMatrix * mViewMatrix;
		//这里caster是观察者,所以传vp
		graphcis->setMat4(shader, ShaderParam::MatrixVP, lpv);
	}

	void ShadowCaster::submit(BaseGraphics* graphcis, Shader* shader)
	{
		auto transform = this->getTransform();
		mViewMatrix = glm::lookAt(transform->getWorldPosition()
								, transform->getWorldPosition() + transform->getForward()
								, transform->getUp());

		auto lpv = mProjectionMatrix * mViewMatrix;
		//这里caster是数据提供者,所以传lightvp
		graphcis->setMat4(shader, ShaderParam::MatrixLightVP, lpv);
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

		mShadwowTexutre = Texture2D::create();
		mShadwowTexutre->setData(width, height
			, TextureInfo(TextureType::Texture2D
				, TextureAttachPosition::DepthComponent
				, TextureFilter::Nearest
				, TextureFilter::Nearest
				, TextureWrap::Clamp_To_Border
				, TextureWrap::Clamp_To_Border
				, TextureChannel::Depth32f
				, TextureChannel::Depth
				, DataType::Float32));

		mFrameBuffer = FrameBuffer::create("Shadow");
		mFrameBuffer->addAttachment(mShadwowTexutre);
		mFrameBuffer->generate();

		mFrameBuffer->addRef();
	}
}
