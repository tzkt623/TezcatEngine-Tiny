#include "EnvironmentLightManager.h"
#include "../Renderer/BaseGraphics.h"
#include "../Renderer/RenderPass.h"
#include "../Renderer/Texture.h"
#include "../Renderer/RenderCommand.h"
#include "../Renderer/Vertex.h"
#include "../Renderer/FrameBuffer.h"

#include "../Shader/Shader.h"
#include "../Shader/ShaderPackage.h"

#include "FrameBufferManager.h"
#include "TextureManager.h"
#include "VertexBufferManager.h"
#include "ShaderManager.h"

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
		//这里不需要传viewmatrix
	}

	void EnvObserver::submit(Shader* shader)
	{

	}

	//----------------------------------------------------------
	//
	// EnvironmentLighting
	//
	EnvironmentLightManager::EnvironmentLightManager()
		: mCubeObserver(nullptr)
		, mIrradianceObserver(nullptr)
		, mPrefilterObserver(nullptr)
		, mBRDFLUTObserver(nullptr)
		, mDirty(false)
		, mSkybox(nullptr)
		, mHDRTexName()
		, mCubeFB(nullptr)
		, mIrradianceFB(nullptr)
		, mPrefilterFB(nullptr)
		, mCubeMap(nullptr)
		, mIrradianceMap(nullptr)
		, mPrefilterMap(nullptr)
		, mBRDFLUTMap(nullptr)
		, mCubeSize(1024)
		, mIrrSize(32)
	{
		EnvLitMgr::attach(this);

		EngineEvent::get()->addListener(EngineEventID::EE_ActiveSkybox
			, this
			, [this](const EventData& data)
			{
				mSkybox = static_cast<Skybox*>(data.userData);
			});

		EngineEvent::get()->addListener(EngineEventID::EE_RenderEnv
			, this
			, [this](const EventData& data)
			{
				mDirty = true;
				mHDRTexName = *static_cast<std::string*>(data.userData);
			});
	}

	EnvironmentLightManager::~EnvironmentLightManager()
	{
		EngineEvent::get()->removeListener(this);
		mCubeObserver->subRef();
	}

	void EnvironmentLightManager::calculate(BaseGraphics* graphics)
	{
		if (mDirty)
		{
			mDirty = false;
			mSkybox->getMaterial()->setUniform<UniformTexCube>(ShaderParam::TexCube, mCubeMap);
			mSkybox->getMaterial()->setUniform<UniformI1>(ShaderParam::IsHDR, true);

			//---------------------------------
			auto cube_vertex = VertexBufMgr::getInstance()->getVertex("Cube");
			auto shader = ShaderMgr::getInstance()->findPackage("Unlit/EnvMakeCube")->getShaders()[0];
			auto hdr = (Texture2D*)TextureMgr::getInstance()->findTexture(mHDRTexName);

			RenderCommand* cmd = new RenderCMD_HDRToCube(cube_vertex
													   , hdr
													   , mCubeMap
													   , shader
													   , mCubeFB);
			mCubeObserver->getRenderQueue()->addRenderCommand(cmd);
			graphics->addPreRenderPassQueue(mCubeObserver->getRenderQueue<ExtraQueue>());

			//---------------------------------
			shader = ShaderMgr::getInstance()->findPackage("Unlit/EnvMakeIrradiance")->getShaders()[0];
			cmd = new RenderCMD_EnvMakeIrradiance(cube_vertex
												, mCubeMap
												, mIrradianceMap
												, shader
												, mIrradianceFB);
			mIrradianceObserver->getRenderQueue()->addRenderCommand(cmd);
			graphics->addPreRenderPassQueue(mIrradianceObserver->getRenderQueue<ExtraQueue>());

			//---------------------------------
			shader = ShaderMgr::getInstance()->findPackage("Unlit/EnvMakePrefilter")->getShaders()[0];
			cmd = new RenderCMD_EnvMakePrefilter(cube_vertex
											   , mCubeMap
											   , mPrefilterMap
											   , shader
											   , mPrefilterFB
											   , 5
											   , 128
											   , 128
											   , mCubeSize);
			mPrefilterObserver->getRenderQueue()->addRenderCommand(cmd);
			graphics->addPreRenderPassQueue(mPrefilterObserver->getRenderQueue<ExtraQueue>());

			//---------------------------------
			auto rect_vertex = VertexBufMgr::getInstance()->getVertex("Rect");
			shader = ShaderMgr::getInstance()->findPackage("Unlit/EnvMakeBRDFLut")->getShaders()[0];
			cmd = new RenderCMD_Vertex(rect_vertex, shader);
			mBRDFLUTObserver->getRenderQueue()->addRenderCommand(cmd);
			graphics->addPreRenderPassQueue(mBRDFLUTObserver->getRenderQueue<ExtraQueue>());
		}
	}

	void EnvironmentLightManager::init()
	{
		this->createCube();
		this->createIrradiance();
		this->createPrefilter();
		this->createBRDF_LUT();
	}

	void EnvironmentLightManager::submit(Shader* shader)
	{
		shader->setGlobalTextureCube(ShaderParam::TexIrradiance, mIrradianceMap);
		shader->setGlobalTextureCube(ShaderParam::TexPrefilter, mPrefilterMap);
		shader->setGlobalTexture2D(ShaderParam::TexBRDFLUT, mBRDFLUTMap);
	}

	void EnvironmentLightManager::createCube()
	{
		int cube_size = mCubeSize;
		mCubeObserver = EnvObserver::create();
		mCubeObserver->setPerspective(90.0f, 0.1f, 10.0f);
		mCubeObserver->setViewRect(0, 0, cube_size, cube_size);
		mCubeObserver->setClearOption(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
		mCubeObserver->addRef();

		mCubeMap = TextureMgr::getInstance()->createCube(
			cube_size, cube_size,
			TextureInfo("CB_CubeMap"
				, TextureType::TextureCube
				, TextureAttachPosition::ColorComponent
				, TextureFilter::Linear_Mipmap_Linear
				, TextureFilter::Linear
				, TextureWrap::Clamp_To_Edge
				, TextureWrap::Clamp_To_Edge
				, TextureWrap::Clamp_To_Edge
				, TextureChannel::RGB16f
				, TextureChannel::RGB
				, DataType::Float32));

// 		auto render2d = TextureMgr::getInstance()->createRender2D(
// 			cube_size, cube_size,
// 			TextureInfo("DB_CubeMap"
// 				, TextureType::TextureRender2D
// 				, TextureAttachPosition::DepthComponent
// 				, TextureChannel::Depth24));

		mCubeFB = FrameBufferMgr::getInstance()->create("FB_Cube");
		mCubeFB->beginBuild();
		mCubeFB->attachCube(mCubeMap, 0, 0);
		//mCubeFB->attach(render2d);
		mCubeFB->endBuild();
		mCubeObserver->setFrameBuffer(mCubeFB);
	}

	void EnvironmentLightManager::createIrradiance()
	{
		const int irr_size = mIrrSize;
		mIrradianceObserver = EnvObserver::create();
		mIrradianceObserver->setPerspective(90.0f, 0.1f, 10.0f);
		mIrradianceObserver->setViewRect(0, 0, irr_size, irr_size);
		mIrradianceObserver->setClearOption(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
		mIrradianceObserver->addRef();

		mIrradianceMap = TextureMgr::getInstance()->createCube(
			irr_size, irr_size,
			TextureInfo("CB_IrradianceMap"
				, TextureType::TextureCube
				, TextureAttachPosition::ColorComponent
				, TextureFilter::Linear
				, TextureFilter::Linear
				, TextureWrap::Clamp_To_Edge
				, TextureWrap::Clamp_To_Edge
				, TextureWrap::Clamp_To_Edge
				, TextureChannel::RGB16f
				, TextureChannel::RGB
				, DataType::Float32));

// 		auto render2d = TextureMgr::getInstance()->createRender2D(
// 			irr_size, irr_size,
// 			TextureInfo("DB_IrradianceMap"
// 				, TextureType::TextureRender2D
// 				, TextureAttachPosition::DepthComponent
// 				, TextureChannel::Depth24));

		mIrradianceFB = FrameBufferMgr::getInstance()->create("FB_Irradiance");
		mIrradianceFB->beginBuild();
		mIrradianceFB->attachCube(mIrradianceMap, 0, 0);
		//mIrradianceFB->attach(render2d);
		mIrradianceFB->endBuild();
		mIrradianceObserver->setFrameBuffer(mIrradianceFB);
	}

	void EnvironmentLightManager::createPrefilter()
	{
		const int prefilter_size = 128;
		mPrefilterObserver = EnvObserver::create();
		mPrefilterObserver->setPerspective(90.0f, 0.1f, 10.0f);
		mPrefilterObserver->setViewRect(0, 0, prefilter_size, prefilter_size);
		mPrefilterObserver->setClearOption(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
		mPrefilterObserver->addRef();

		mPrefilterMap = TextureMgr::getInstance()->createCube(
			prefilter_size, prefilter_size,
			TextureInfo("CB_PrefilterMap"
				, TextureType::TextureCube
				, TextureAttachPosition::ColorComponent
				, TextureFilter::Linear_Mipmap_Linear
				, TextureFilter::Linear
				, TextureWrap::Clamp_To_Edge
				, TextureWrap::Clamp_To_Edge
				, TextureWrap::Clamp_To_Edge
				, TextureChannel::RGB16f
				, TextureChannel::RGB
				, DataType::Float32));

// 		mRender2D = TextureMgr::getInstance()->createRender2D(
// 			prefilter_size, prefilter_size,
// 			TextureInfo("DB_PrefilterMap"
// 				, TextureType::TextureRender2D
// 				, TextureAttachPosition::DepthComponent
// 				, TextureChannel::Depth24));

		mPrefilterFB = FrameBufferMgr::getInstance()->create("FB_Prefilter");
		mPrefilterFB->beginBuild();
		mPrefilterFB->attachCube(mPrefilterMap, 0, 0);
		//mPrefilterFB->attach(mRender2D);
		mPrefilterFB->endBuild();

		mPrefilterObserver->setFrameBuffer(mPrefilterFB);
	}

	void EnvironmentLightManager::createBRDF_LUT()
	{
		mBRDFLUTObserver = EnvObserver::create();
		mBRDFLUTObserver->setPerspective(90.0f, 0.1f, 10.0f);
		mBRDFLUTObserver->setViewRect(0, 0, mCubeSize, mCubeSize);
		mBRDFLUTObserver->setClearOption(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
		mBRDFLUTObserver->addRef();

		auto fb = FrameBufferMgr::getInstance()->create("FB_BRDF_LUT",
			mCubeSize, mCubeSize,
			{
				TextureInfo("CB_BRDF_LUT"
					, TextureType::Texture2D
					, TextureAttachPosition::ColorComponent
					, TextureFilter::Linear
					, TextureFilter::Linear
					, TextureWrap::Clamp_To_Edge
					, TextureWrap::Clamp_To_Edge
					, TextureChannel::RG16f
					, TextureChannel::RG
					, DataType::Float32),

// 				TextureInfo("DB_BRDF_LUT"
// 					, TextureType::TextureRender2D
// 					, TextureAttachPosition::DepthComponent
// 					, TextureChannel::Depth24)
			});

		mBRDFLUTMap = (Texture2D*)TextureMgr::getInstance()->findTexture("CB_BRDF_LUT");
		mBRDFLUTObserver->setFrameBuffer(fb);
	}
	void EnvironmentLightManager::showIrradianceMap()
	{
		mSkybox->getMaterial()->setUniform<UniformTexCube>(ShaderParam::TexCube, mIrradianceMap);
	}

	void EnvironmentLightManager::showPrefilterMap()
	{
		mSkybox->getMaterial()->setUniform<UniformTexCube>(ShaderParam::TexCube, mPrefilterMap);
	}

	void EnvironmentLightManager::showSkybox()
	{
		mSkybox->getMaterial()->setUniform<UniformTexCube>(ShaderParam::TexCube, mCubeMap);
	}
}
