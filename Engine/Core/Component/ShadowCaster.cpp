#include "ShadowCaster.h"
#include "Transform.h"
#include "GameObject.h"

#include "../Shader/ShaderParam.h"
#include "../Shader/Shader.h"

#include "../Manager/FrameBufferManager.h"
#include "../Manager/TextureManager.h"
#include "../Manager/ShaderManager.h"

#include "../Renderer/Texture.h"
#include "../Renderer/FrameBuffer.h"
#include "../Renderer/BaseGraphics.h"
#include "../Renderer/Pipeline.h"

#include "../Manager/ShadowCasterManager.h"

namespace tezcat::Tiny
{
	TINY_OBJECT_CPP(ShadowCaster, ComponentT<ShadowCaster>)

	ShadowCaster::ShadowCaster()
		: mShadowObserver()
		, mShadowTexture(nullptr)
		, mPipePass(nullptr)
		, mFrameBuffer(nullptr)
		, mUID(0)
	{

	}

	ShadowCaster::~ShadowCaster()
	{

	}

	void ShadowCaster::init()
	{
		Base::init();

		mShadowObserver = RenderObserver::create();
		mShadowObserver->saveObject();
		mShadowObserver->setOrderID(-126);

		mPipePass = ReplacedPipelinePass::create(mShadowObserver
			, ShaderManager::find("Unlit/ShadowMap"));
		mPipePass->setUseCullLayerData(true);
		mPipePass->saveObject();
	}

	void ShadowCaster::onStart()
	{
		mShadowObserver->setTransform(mGameObject->getTransform());
	}

	void ShadowCaster::onEnable()
	{
		mUID = ShadowCasterManager::add(this);
		mPipePass->addToPipeline();
	}

	void ShadowCaster::onDisable()
	{
		ShadowCasterManager::recycle(this);
		mPipePass->removeFromPipeline();
		mUID = 0;
	}

	void ShadowCaster::onDestroy()
	{
		ShadowCasterManager::recycle(this);
		mUID = 0;

		mFrameBuffer->deleteObject();
		mShadowObserver->deleteObject();
		mShadowTexture->deleteObject();
		mPipePass->deleteObject();
	}

	void ShadowCaster::setShadowMap(int width, int height, const std::string& shaderName)
	{
		if (mFrameBuffer)
		{
			return;
		}

		mShadowTexture = Texture2D::create("Shadow");
		mShadowTexture->saveObject();
		mShadowTexture->setConfig(width, height
			, TextureInternalFormat::Depth
			, TextureFormat::Depth
			, DataMemFormat::Float);
		mShadowTexture->setAttachPosition(TextureAttachPosition::DepthComponent);

		mFrameBuffer = FrameBuffer::create(shaderName + std::to_string(mUID));
		mFrameBuffer->saveObject();
		mFrameBuffer->addAttachment(mShadowTexture);
		mFrameBuffer->generate();

		mShadowObserver->setFrameBuffer(mFrameBuffer);
		mShadowObserver->setViewRect(0, 0, width, height);
	}

	void ShadowCaster::submit(BaseGraphics* graphics, Shader* shader)
	{
		auto pv = mShadowObserver->getProjectionMatrix() * mShadowObserver->getViewMatrix();
		graphics->setMat4(shader, ShaderParam::MatrixLightVP, pv);
		graphics->setGlobalTexture2D(shader, ShaderParam::TexDepth, mShadowTexture);
	}

}
