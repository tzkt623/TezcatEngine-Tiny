#include "EnvironmentLighting.h"
#include "BaseGraphics.h"
#include "RenderPass.h"
#include "Texture.h"
#include "RenderCommand.h"
#include "Vertex.h"
#include "FrameBuffer.h"

#include "../Shader/Shader.h"
#include "../Shader/ShaderPackage.h"

#include "../Manager/FrameBufferManager.h"
#include "../Manager/TextureManager.h"
#include "../Manager/BufferManager.h"
#include "../Manager/ShaderManager.h"

#include "../Event/EngineEvent.h"

#include "../Component/Skybox.h"

#include "../Data/Material.h"

namespace tezcat::Tiny
{
	TINY_RTTI_CPP(EnvObserver);
	EnvObserver::EnvObserver()
		: IRenderObserver(new ExtraQueue(this))
	{
		this->setViewRect(0, 0, 540, 540);
	}

	EnvObserver::~EnvObserver()
	{

	}

	void EnvObserver::submitViewMatrix(Shader* shader)
	{
		this->updateObserverMatrix();
		shader->setProjectionMatrix(mProjectionMatrix);
	}

	void EnvObserver::submit(Shader* shader)
	{

	}

	//----------------------------------------------------------
	//
	// EnvironmentLighting
	//
	EnvironmentLighting::EnvironmentLighting()
		: mToCubeObserver(nullptr)
		, mToEnvObserver(nullptr)
		, mDirty(false)
		, mSkybox(nullptr)
		, mHDRTexName()
		, mToCubeFB(nullptr)
		, mToEnvFB(nullptr)
		, mCubeMap(nullptr)
		, mEnvMap(nullptr)
	{
		EnvLighting::attach(this);

		EngineEvent::get()->addListener(EngineEventID::EE_ActiveSkybox
			, this
			, [this](const EventData& data)
			{
				mSkybox = static_cast<Skybox*>(data.userData);
			});
	}

	EnvironmentLighting::~EnvironmentLighting()
	{
		EngineEvent::get()->removeListener(this);
		mToCubeObserver->subRef();
	}

	void EnvironmentLighting::calculate(BaseGraphics* graphics)
	{
		if (mDirty)
		{
			mDirty = false;
			mSkybox->getMaterial()->setUniform<UniformTexCube>(ShaderParam::TexCube, mCubeMap);
			mSkybox->getMaterial()->setUniform<UniformI1>(ShaderParam::IsHDR, true);

			auto vertex = BufferMgr::getInstance()->getVertex("Cube");
			auto shader = ShaderMgr::getInstance()->findPackage("Unlit/EnvMakeCube")->getShaders()[0];
			auto hdr = (Texture2D*)TextureMgr::getInstance()->findTexture(mHDRTexName);

			RenderCommand* cmd = new RenderCMD_HDRToCube(vertex, hdr, mCubeMap, shader, mToCubeFB);
			mToCubeObserver->getRenderQueue()->addRenderCommand(cmd);
			graphics->addPreRenderPassQueue(mToCubeObserver->getRenderQueue<ExtraQueue>());

			shader = ShaderMgr::getInstance()->findPackage("Unlit/EnvMap")->getShaders()[0];
			cmd = new RenderCMD_EnvMap(vertex, mCubeMap, mEnvMap, shader, mToEnvFB);
			mToEnvObserver->getRenderQueue()->addRenderCommand(cmd);
			graphics->addPreRenderPassQueue(mToEnvObserver->getRenderQueue<ExtraQueue>());
		}
	}

	void EnvironmentLighting::init()
	{
		const int cube_size = 512;
		mToCubeObserver = EnvObserver::create();
		mToCubeObserver->setPerspective(90.0f, 0.1f, 10.0f);
		mToCubeObserver->setViewRect(0, 0, cube_size, cube_size);
		mToCubeObserver->setClearOption(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
		mToCubeObserver->addRef();

		mCubeMap = TextureMgr::getInstance()->createCube(cube_size, cube_size,
			TextureInfo("CB_ToCube"
				, TextureType::TextureCube
				, TextureAttachPosition::ColorComponent
				, TextureFilter::Linear
				, TextureWrap::Clamp_To_Edge
				, TextureChannel::RGBF16
				, TextureChannel::RGBA
				, DataType::Float32));

		auto render2d = TextureMgr::getInstance()->createRender2D(cube_size, cube_size,
			TextureInfo("DB_ToCube"
				, TextureType::TextureRender2D
				, TextureAttachPosition::DepthComponent
				, TextureChannel::Depth24));

		mToCubeFB = FrameBufferMgr::getInstance()->create("FB_ToCubeMap");
		mToCubeFB->beginBuild();
		mToCubeFB->attachCube(mCubeMap, 0, 0);
		mToCubeFB->attach(render2d);
		mToCubeFB->endBuild();
		mToCubeObserver->setFrameBuffer(mToCubeFB);


		const int env_size = 32;
		mToEnvObserver = EnvObserver::create();
		mToEnvObserver->setPerspective(90.0f, 0.1f, 10.0f);
		mToEnvObserver->setViewRect(0, 0, env_size, env_size);
		mToEnvObserver->setClearOption(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
		mToEnvObserver->addRef();

		mEnvMap = TextureMgr::getInstance()->createCube(env_size, env_size,
			TextureInfo("CB_ToEnv"
				, TextureType::TextureCube
				, TextureAttachPosition::ColorComponent
				, TextureFilter::Linear
				, TextureWrap::Clamp_To_Edge
				, TextureChannel::RGBF16
				, TextureChannel::RGBA
				, DataType::Float32));

		render2d = TextureMgr::getInstance()->createRender2D(env_size, env_size,
			TextureInfo("DB_ToEnv"
				, TextureType::TextureRender2D
				, TextureAttachPosition::DepthComponent
				, TextureChannel::Depth24));

		mToEnvFB = FrameBufferMgr::getInstance()->create("FB_ToEnvMap");
		mToEnvFB->beginBuild();
		mToEnvFB->attachCube(mEnvMap, 0, 0);
		mToEnvFB->attach(render2d);
		mToEnvFB->endBuild();
		mToEnvObserver->setFrameBuffer(mToEnvFB);
	}

	void EnvironmentLighting::setDirty(const std::string& texName)
	{
		mDirty = true;
		mHDRTexName = texName;
	}

	void EnvironmentLighting::submit(Shader* shader)
	{
		shader->setGlobalTextureCube(ShaderParam::TexEnv, mEnvMap);
	}

}
