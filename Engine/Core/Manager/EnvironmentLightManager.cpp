#include "EnvironmentLightManager.h"
#include "FrameBufferManager.h"
#include "TextureManager.h"
#include "VertexBufferManager.h"
#include "ShaderManager.h"

#include "../Renderer/BaseGraphics.h"
#include "../Renderer/RenderPass.h"
#include "../Renderer/Texture.h"
#include "../Renderer/RenderCommand.h"
#include "../Renderer/Vertex.h"
#include "../Renderer/FrameBuffer.h"

#include "../Shader/Shader.h"
#include "../Shader/ShaderPackage.h"

#include "../Event/EngineEvent.h"
#include "../Data/Material.h"
#include "../Data/Image.h"

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

	void EnvObserver::submitViewMatrix(BaseGraphics* graphics, Shader* shader)
	{
		this->updateObserverMatrix();
		graphics->setMat4(shader, ShaderParam::MatrixP, mProjectionMatrix);
		//这里不需要传viewmatrix
	}

	void EnvObserver::submit(BaseGraphics* graphics, Shader* shader)
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
		, mTexHDR(nullptr)
		, mCubeFB(nullptr)
		, mIrradianceFB(nullptr)
		, mPrefilterFB(nullptr)
		, mCubeMap(nullptr)
		, mIrradianceMap(nullptr)
		, mPrefilterMap(nullptr)
		, mBRDFLUTMap(nullptr)
		, mCubeSize(1024)
		, mIrrSize(32)
		, mPrefilterSize(128)
		, mSkyboxLod(0)
		, mSkyboxShader(nullptr)
		, mSkyboxVertex(nullptr)
	{
		EnvLitMgr::attach(this);

		EngineEvent::get()->addListener(EngineEventID::EE_ChangeEnvLightingImage
			, this
			, [this](const EventData& data)
			{
				mDirty = true;
				this->setHDRImage(static_cast<Image*>(data.userData));
			});

		EngineEvent::get()->addListener(EngineEventID::EE_ChangeEnvLightingTexture
			, this
			, [this](const EventData& data)
			{
				mDirty = true;
				if (mTexHDR)
				{
					mTexHDR->subRef();
				}

				mTexHDR = static_cast<Texture2D*>(data.userData);
				mTexHDR->addRef();
			});

		EngineEvent::get()->addListener(EngineEventID::EE_OnPushScene
			, this
			, [this](const EventData& data)
			{
				mDirty = false;
				if (mTexHDR)
				{
					mTexHDR->subRef();
					mTexHDR = nullptr;
				}
			});

		EngineEvent::get()->addListener(EngineEventID::EE_OnPopScene
			, this
			, [this](const EventData& data)
			{
				mDirty = false;
				if (mTexHDR)
				{
					mTexHDR->subRef();
					mTexHDR = nullptr;
				}
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
			mCurrentCubeMap = mCubeMap;
			//---------------------------------
			auto cube_vertex = VertexBufMgr::getInstance()->create("Cube");
			auto shader = ShaderMgr::getInstance()->find("Unlit/EnvMakeCube");

			auto cmd = graphics->createDrawHDRToCubeCMD(shader, cube_vertex, mTexHDR, mCubeMap);
			mCubeObserver->getRenderQueue()->addRenderCommand(cmd);
			graphics->addPreRenderPassQueue(mCubeObserver->getRenderQueue<ExtraQueue>());

			//---------------------------------
			shader = ShaderMgr::getInstance()->find("Unlit/EnvMakeIrradiance");
			cmd = graphics->createDrawEnvMakeIrradiance(shader, cube_vertex, mCubeMap, mIrradianceMap);
			mIrradianceObserver->getRenderQueue()->addRenderCommand(cmd);
			graphics->addPreRenderPassQueue(mIrradianceObserver->getRenderQueue<ExtraQueue>());

			//---------------------------------
			shader = ShaderMgr::getInstance()->find("Unlit/EnvMakePrefilter");
			cmd = graphics->createDrawEnvMakePrefilter(shader
													 , cube_vertex
													 , mCubeMap
													 , mPrefilterMap
													 , 5
													 , mPrefilterSize
													 , mPrefilterSize
													 , mCubeSize);
			mPrefilterObserver->getRenderQueue()->addRenderCommand(cmd);
			graphics->addPreRenderPassQueue(mPrefilterObserver->getRenderQueue<ExtraQueue>());

			//---------------------------------
			auto rect_vertex = VertexBufMgr::getInstance()->create("Rect");
			shader = ShaderMgr::getInstance()->find("Unlit/EnvMakeBRDFLut");
			cmd = graphics->createDrawVertexCMD(shader, rect_vertex);
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

	void EnvironmentLightManager::submit(BaseGraphics* graphics, Shader* shader)
	{
		graphics->setGlobalTextureCube(shader, ShaderParam::TexIrradiance, mIrradianceMap);
		graphics->setGlobalTextureCube(shader, ShaderParam::TexPrefilter, mPrefilterMap);
		graphics->setGlobalTexture2D(shader, ShaderParam::TexBRDFLUT, mBRDFLUTMap);
	}

	void EnvironmentLightManager::createCube()
	{
		int cube_size = mCubeSize;
		mCubeObserver = EnvObserver::create();
		mCubeObserver->setPerspective(90.0f, 0.1f, 10.0f);
		mCubeObserver->setViewRect(0, 0, cube_size, cube_size);
		mCubeObserver->setClearOption(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
		mCubeObserver->addRef();

		mCubeFB = FrameBufferMgr::getInstance()->create("FB_Cube");
		mCubeMap = TextureMgr::getInstance()->createCube("CB_CubeMap");
		mCubeMap->setData(cube_size, cube_size,
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
				, DataType::Float32));		mCubeFB->addAttachment(mCubeMap);
		mCubeFB->generate();

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


		mIrradianceFB = FrameBufferMgr::getInstance()->create("FB_Irradiance");
		mIrradianceMap = TextureMgr::getInstance()->createCube("CB_IrradianceMap");
		mIrradianceMap->setData(irr_size, irr_size,
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

		mIrradianceFB->addAttachment(mIrradianceMap);
		mIrradianceFB->generate();


		mIrradianceObserver->setFrameBuffer(mIrradianceFB);
	}

	void EnvironmentLightManager::createPrefilter()
	{
		const int prefilter_size = mPrefilterSize;
		mPrefilterObserver = EnvObserver::create();
		mPrefilterObserver->setPerspective(90.0f, 0.1f, 10.0f);
		mPrefilterObserver->setViewRect(0, 0, prefilter_size, prefilter_size);
		mPrefilterObserver->setClearOption(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
		mPrefilterObserver->addRef();

		mPrefilterFB = FrameBufferMgr::getInstance()->create("FB_Prefilter");
		mPrefilterMap = TextureMgr::getInstance()->createCube("CB_PrefilterMap");
		mPrefilterMap->setData(prefilter_size, prefilter_size,
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

		mPrefilterFB->addAttachment(mPrefilterMap);
		mPrefilterFB->generate();

		mPrefilterObserver->setFrameBuffer(mPrefilterFB);
	}

	void EnvironmentLightManager::createBRDF_LUT()
	{
		mBRDFLUTObserver = EnvObserver::create();
		mBRDFLUTObserver->setPerspective(90.0f, 0.1f, 10.0f);
		mBRDFLUTObserver->setViewRect(0, 0, mCubeSize, mCubeSize);
		mBRDFLUTObserver->setClearOption(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
		mBRDFLUTObserver->addRef();


		auto fb = FrameBuffer::create("FB_BRDF_LUT");
		mBRDFLUTMap = TextureMgr::getInstance()->create2D("CB_BRDF_LUT");
		mBRDFLUTMap->setData(mCubeSize, mCubeSize,
			TextureInfo("CB_BRDF_LUT"
				, TextureType::Texture2D
				, TextureAttachPosition::ColorComponent
				, TextureFilter::Linear
				, TextureFilter::Linear
				, TextureWrap::Clamp_To_Edge
				, TextureWrap::Clamp_To_Edge
				, TextureChannel::RG16f
				, TextureChannel::RG
				, DataType::Float32));

		fb->addAttachment(mBRDFLUTMap);
		fb->generate();

		mBRDFLUTObserver->setFrameBuffer(fb);
	}
	void EnvironmentLightManager::showIrradianceMap()
	{
		mCurrentCubeMap = mIrradianceMap;
	}

	void EnvironmentLightManager::showPrefilterMap()
	{
		mCurrentCubeMap = mPrefilterMap;
	}

	void EnvironmentLightManager::showSkybox()
	{
		mCurrentCubeMap = mCubeMap;
	}

	void EnvironmentLightManager::setSkyboxLod(float skyboxLod)
	{
		mSkyboxLod = skyboxLod;
	}

	void EnvironmentLightManager::setHDRImage(Image* image)
	{
		TinyAssert(image != nullptr);

		if (mTexHDR)
		{
			mTexHDR->updateData(image);
			mTexHDR->update();
		}
		else
		{
			mTexHDR = Texture2D::create();
			mTexHDR->setData(image);
			mTexHDR->generate();
			mTexHDR->addRef();
		}
	}

	RenderCommand* EnvironmentLightManager::createSkyboxCMD(BaseGraphics* graphics)
	{
		if (mSkyboxShader == nullptr)
		{
			mSkyboxVertex = VertexBufMgr::getInstance()->create("Skybox");
			mSkyboxShader = ShaderMgr::getInstance()->find("Unlit/Skybox");
		}

		if (mTexHDR)
		{
			return graphics->createDrawSkyboxCMD(mSkyboxShader, mSkyboxVertex, mCurrentCubeMap, mSkyboxLod, true);
		}
		else
		{
			return graphics->createDrawSkyboxCMD(mSkyboxShader, mSkyboxVertex, mCurrentCubeMap, mSkyboxLod, false);
		}
	}

}
