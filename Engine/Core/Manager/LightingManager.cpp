#include "LightingManager.h"
#include "FrameBufferManager.h"
#include "TextureManager.h"
#include "VertexBufferManager.h"
#include "ShaderManager.h"
#include "CameraManager.h"

#include "../Component/Transform.h"
#include "../Component/Light.h"
#include "../Component/Camera.h"

#include "../Renderer/RenderObserver.h"
#include "../Renderer/BaseGraphics.h"
#include "../Renderer/Texture.h"
#include "../Renderer/RenderCommand.h"
#include "../Renderer/Vertex.h"
#include "../Renderer/FrameBuffer.h"
#include "../Renderer/Pipeline.h"

#include "../Shader/Shader.h"
#include "../Shader/ShaderPackage.h"

#include "../Event/EngineEvent.h"
#include "../Data/Material.h"
#include "../Data/Image.h"
#include "../Data/Resource.h"

namespace tezcat::Tiny
{
	ReplacedPipelinePass* LightingManager::sBRDFLUTPass = nullptr;
	ReplacedPipelinePass* LightingManager::sPrefilterPass = nullptr;
	ReplacedPipelinePass* LightingManager::sIrradiancePass = nullptr;
	ReplacedPipelinePass* LightingManager::sTexturePass = nullptr;
	ReplacedPipelinePass* LightingManager::sSkyBoxPass = nullptr;

	Texture2D* LightingManager::sTexHDR = nullptr;
	Texture2D* LightingManager::sBRDFLUTMap = nullptr;
	TextureCube* LightingManager::sCubeTextureMap = nullptr;
	TextureCube* LightingManager::sPrefilterMap = nullptr;
	TextureCube* LightingManager::sIrradianceMap = nullptr;
	TextureCube* LightingManager::sCurrentCubeMap = nullptr;
	Texture2D* LightingManager::sCubeTextures[6] = { nullptr };
	Vertex* LightingManager::sSkyboxVertex = nullptr;
	Vertex* LightingManager::sHDRVertex = nullptr;
	LightData* LightingManager::sLightData = nullptr;


	bool LightingManager::sCloseEnvLighting = true;
	float LightingManager::sSkyboxLod = 0;
	uint32 LightingManager::sCubeSize = 1024;
	uint32 LightingManager::sPrefilterSize = 128;
	uint32 LightingManager::sIrrSize = 32;
	bool LightingManager::sCalculateEnvLighting = false;
	bool LightingManager::sEnableSkyBox = false;
	bool LightingManager::sLocker = true;
	float LightingManager::sExposure = 1;
	std::function<void()> LightingManager::sRemovePassFromPipeline = nullptr;

	void LightingManager::init()
	{
		EngineEvent::getInstance()->addListener(EngineEventID::EE_ChangeEnvImage
			, &sLocker
			, [](const EventData& data)
			{
				setSkyBoxHDRTexture(static_cast<Image*>(data.userData));
				sCalculateEnvLighting = true;
			});

		EngineEvent::getInstance()->addListener(EngineEventID::EE_OnPushScene
			, &sLocker
			, [](const EventData& data)
			{
				sLocker = false;
			});

		EngineEvent::getInstance()->addListener(EngineEventID::EE_OnPopScene
			, &sLocker
			, [](const EventData& data)
			{
				sLocker = true;

				if (sTexHDR)
				{
					sTexHDR->deleteObject();
					sTexHDR = nullptr;
				}

				sTexturePass->removeFromPipeline();
				sIrradiancePass->removeFromPipeline();
				sPrefilterPass->removeFromPipeline();
				sBRDFLUTPass->removeFromPipeline();

				if (sSkyBoxPass)
				{
					sSkyBoxPass->removeFromPipeline();
					sSkyBoxPass->deleteObject();
					sSkyBoxPass = nullptr;
				}

				sCurrentCubeMap = nullptr;
			});

		sSkyboxVertex = VertexBufferManager::create("Skybox");
		sHDRVertex = VertexBufferManager::create("Cube");
		createCube();
		createIrradiance();
		createPrefilter();
		createBRDF_LUT();
	}

	void LightingManager::close()
	{
		EngineEvent::getInstance()->removeListener(&sLocker);
	}

	void LightingManager::calculate()
	{
		if (sLocker)
		{
			return;
		}

		if (sCalculateEnvLighting)
		{
			sCalculateEnvLighting = false;
			sTexturePass->addToPipeline();
			sIrradiancePass->addToPipeline();
			sPrefilterPass->addToPipeline();
			sBRDFLUTPass->addToPipeline();
		}

		if (sEnableSkyBox)
		{
			if (sSkyBoxPass == nullptr)
			{
				if (CameraManager::getMainCamera())
				{
					auto shader = ShaderManager::find("Unlit/Skybox");
					sSkyBoxPass = ReplacedPipelinePass::create(CameraManager::getMainCamera()->getRenderObserver()
						, shader);
					sSkyBoxPass->saveObject();
					if (sCurrentCubeMap == nullptr)
					{
						sCurrentCubeMap = sCubeTextureMap;
					}

// 					auto material = Material::create(shader);
// 					material->saveObject();
// 
// 					auto skybox_lod_index = shader->getUniformIndex("mySkyboxLod");
// 					auto is_hdr_index = shader->getUniformIndex("myIsHDR");
// 					auto exposure_index = shader->getUniformIndex("myExposure");

					sSkyBoxPass->setPreFunction([=](ReplacedPipelinePass* pass)
					{
// 						material->setTinyUniform<UniformTexCube>(ShaderParam::TexSkybox, sCurrentCubeMap);
// 						material->setUniform<UniformI1>(skybox_lod_index, sSkyboxLod);
// 						material->setUniform<UniformI1>(is_hdr_index, sCurrentCubeMap->getDataMemFormat().tiny == DataMemFormat::Float);
// 						material->setUniform<UniformF1>(exposure_index, sExposure);

						auto cmd = Graphics::getInstance()->createDrawSkyboxCMD(sSkyboxVertex
							, sCurrentCubeMap
							, sSkyboxLod
							, sCurrentCubeMap->getDataMemFormat().tiny == DataMemFormat::Float
							, sExposure);

						//new RenderCommand(sSkyboxVertex, nullptr, material);
						pass->addCommand(cmd);
					});
				}
			}

			if (sSkyBoxPass)
			{
				sTexturePass->addToPipeline();
				sSkyBoxPass->addToPipeline();
			}
		}
		else
		{
			if (sSkyBoxPass)
			{
				sSkyBoxPass->removeFromPipeline();
			}
		}
	}

	void LightingManager::submitEnvLighting(Shader* shader)
	{
		Graphics::getInstance()->setGlobalTextureCube(shader, ShaderParam::TexIrradiance, sIrradianceMap);
		Graphics::getInstance()->setGlobalTextureCube(shader, ShaderParam::TexPrefilter, sPrefilterMap);
		Graphics::getInstance()->setGlobalTexture2D(shader, ShaderParam::TexBRDFLUT, sBRDFLUTMap);
	}

	void LightingManager::createCube()
	{
		int cube_size = sCubeSize;

		auto [flag2, frame_buffer] = FrameBufferManager::create("FB_Cube");
		if (flag2)
		{
			for (uint32 i = 0; i < 6; i++)
			{
				sCubeTextures[i] = Texture2D::create("CubeTexture" + std::to_string(i));
				sCubeTextures[i]->setConfig(cube_size, cube_size
					, TextureInternalFormat::RGB16F
					, TextureFormat::RGB
					, DataMemFormat::Float);
				sCubeTextures[i]->generate();
				sCubeTextures[i]->saveObject();
			}

			sCubeTextureMap = TextureCube::create("CB_CubeMap");
			sCubeTextureMap->setConfig(cube_size
				, TextureInternalFormat::RGB16F
				, TextureFormat::RGB
				, DataMemFormat::Float
				, TextureFilter::Linear_Mipmap_Linear
				, TextureFilter::Linear);
			sCubeTextureMap->setAttachPosition(TextureAttachPosition::ColorComponent);
			sCurrentCubeMap = sCubeTextureMap;

			auto render_buffer = Texture2D::create("Depth");
			render_buffer->setConfig(cube_size, cube_size
				, TextureInternalFormat::Depth
				, TextureFormat::Depth
				, DataMemFormat::Float);
			render_buffer->setAttachPosition(TextureAttachPosition::DepthComponent);

			frame_buffer->addAttachment(sCubeTextureMap);
			frame_buffer->addAttachment(render_buffer);
			frame_buffer->generate();
		}

		auto observer = createObserver();
		observer->setPerspective(90.0f, 0.1f, 10.0f);
		observer->setViewRect(0, 0, cube_size, cube_size);
		observer->setClearOption(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
		observer->setOrderID(-127);

		sTexturePass = ReplacedPipelinePass::create(observer, ShaderManager::find("Unlit/EnvMakeCube"));
		sTexturePass->setOnceMode();
		sTexturePass->setFrameBuffer(frame_buffer);
		sTexturePass->setPreFunction([=](ReplacedPipelinePass* pass)
		{
			pass->addCommand(Graphics::getInstance()->createDrawHDRToCubeCMD(sHDRVertex, sTexHDR, sCubeTextureMap));
		});
		sTexturePass->setOrderID(0);
		sTexturePass->saveObject();
	}

	void LightingManager::createIrradiance()
	{
		const auto irr_size = sIrrSize;

		auto [flag1, frame_buffer] = FrameBufferManager::create("FB_Irradiance");
		if (flag1)
		{
			auto [flag2, texture] = TextureManager::createCube("CB_IrradianceMap");
			if (flag2)
			{
				texture->setConfig(irr_size
					, TextureInternalFormat::RGB16F
					, TextureFormat::RGB
					, DataMemFormat::Float
					, TextureFilter::Linear_Mipmap_Linear
					, TextureFilter::Linear);
				texture->setAttachPosition(TextureAttachPosition::ColorComponent);
				sIrradianceMap = texture;
			}

			frame_buffer->addAttachment(texture);
			frame_buffer->generate();
		}

		auto observer = createObserver();
		observer->setPerspective(90.0f, 0.1f, 10.0f);
		observer->setViewRect(0, 0, irr_size, irr_size);
		observer->setClearOption(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
		observer->setOrderID(-127);

		auto shader = ShaderManager::find("Unlit/EnvMakeIrradiance");

		sIrradiancePass = ReplacedPipelinePass::create(observer, shader);
		sIrradiancePass->setOnceMode();
		sIrradiancePass->setFrameBuffer(frame_buffer);
		sIrradiancePass->setPreFunction([=](ReplacedPipelinePass* pass)
		{
			pass->addCommand(Graphics::getInstance()->createDrawEnvMakeIrradiance(sHDRVertex, sCubeTextureMap, sIrradianceMap));
		});
		sIrradiancePass->setOrderID(1);
		sIrradiancePass->saveObject();
	}

	void LightingManager::createPrefilter()
	{
		const int prefilter_size = sPrefilterSize;

		auto [flag1, frame_buffer] = FrameBufferManager::create("FB_Prefilter");
		if (flag1)
		{
			auto [flag2, texture] = TextureManager::createCube("CB_PrefilterMap");
			if (flag2)
			{
				texture->setConfig(prefilter_size
					, TextureInternalFormat::RGB16F
					, TextureFormat::RGB
					, DataMemFormat::Float
					, TextureFilter::Linear_Mipmap_Linear
					, TextureFilter::Linear);

				texture->setAttachPosition(TextureAttachPosition::ColorComponent);
				sPrefilterMap = texture;
			}

			frame_buffer->addAttachment(texture);
			frame_buffer->generate();
		}


		auto render_observer = createObserver();
		render_observer->setPerspective(90.0f, 0.1f, 10.0f);
		render_observer->setViewRect(0, 0, prefilter_size, prefilter_size);
		render_observer->setClearOption(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
		render_observer->setOrderID(-127);

		sPrefilterPass = ReplacedPipelinePass::create(render_observer
			, ShaderManager::find("Unlit/EnvMakePrefilter"));
		sPrefilterPass->setOnceMode();
		sPrefilterPass->setFrameBuffer(frame_buffer);
		sPrefilterPass->setPreFunction([=](ReplacedPipelinePass* pass)
		{
			pass->addCommand(Graphics::getInstance()->createDrawEnvMakePrefilter(sHDRVertex
				, sCubeTextureMap
				, sPrefilterMap
				, 5
				, prefilter_size
				, prefilter_size
				, sCubeSize));
		});
		sPrefilterPass->setOrderID(2);
		sPrefilterPass->saveObject();
	}

	void LightingManager::createBRDF_LUT()
	{
		auto [flag1, frame_buffer] = FrameBufferManager::create("FB_BRDF_LUT");
		if (flag1)
		{
			auto [flag2, texture] = TextureManager::create2D("CB_BRDF_LUT");
			if (flag2)
			{
				texture->setConfig(sCubeSize, sCubeSize
					, TextureInternalFormat::RG16F
					, TextureFormat::RG
					, DataMemFormat::Float);
				texture->setAttachPosition(TextureAttachPosition::ColorComponent);
				sBRDFLUTMap = texture;
			}

			frame_buffer->addAttachment(texture);
			frame_buffer->generate();
		}

		auto render_observer = createObserver();
		render_observer->setPerspective(90.0f, 0.1f, 10.0f);
		render_observer->setViewRect(0, 0, sCubeSize, sCubeSize);
		render_observer->setClearOption(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
		render_observer->setOrderID(-127);

		sBRDFLUTPass = ReplacedPipelinePass::create(render_observer
			, ShaderManager::find("Unlit/EnvMakeBRDFLut"));
		sBRDFLUTPass->setOnceMode();
		sBRDFLUTPass->setFrameBuffer(frame_buffer);
		sBRDFLUTPass->setPreFunction([=](ReplacedPipelinePass* pass)
		{
			auto rect_vertex = VertexBufferManager::create("Rect");
			pass->addCommand(Graphics::getInstance()->createDrawVertexCMD(rect_vertex));
		});
		sBRDFLUTPass->setOrderID(3);
		sBRDFLUTPass->saveObject();
	}
	void LightingManager::showIrradianceMap()
	{
		sCurrentCubeMap = sIrradianceMap;
	}

	void LightingManager::showPrefilterMap()
	{
		sCurrentCubeMap = sPrefilterMap;
	}

	void LightingManager::showSkybox()
	{
		sCurrentCubeMap = sCubeTextureMap;
	}

	void LightingManager::setSkyboxLod(float skyboxLod)
	{
		sSkyboxLod = skyboxLod;
	}

	void LightingManager::setSkyBoxHDRTexture(Image* image)
	{
		TINY_ASSERT(image != nullptr);

		if (sTexHDR)
		{
			sTexHDR->setImage(image);
			sTexHDR->generate();
		}
		else
		{
			sTexHDR = Texture2D::create();
			sTexHDR->setImage(image);
			sTexHDR->generate();
			sTexHDR->saveObject();
		}
	}

	RenderObserverMultView* LightingManager::createObserver()
	{
		RenderObserverMultView* p = RenderObserverMultView::create();
		auto array = new float4x4[6]
		{
			glm::lookAt(float3(0.0f, 0.0f, 0.0f), float3(1.0f,  0.0f,  0.0f), float3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(float3(0.0f, 0.0f, 0.0f), float3(-1.0f, 0.0f,  0.0f), float3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(float3(0.0f, 0.0f, 0.0f), float3(0.0f,  1.0f,  0.0f), float3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(float3(0.0f, 0.0f, 0.0f), float3(0.0f, -1.0f,  0.0f), float3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(float3(0.0f, 0.0f, 0.0f), float3(0.0f,  0.0f,  1.0f), float3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(float3(0.0f, 0.0f, 0.0f), float3(0.0f,  0.0f, -1.0f), float3(0.0f, -1.0f,  0.0f))
		};
		p->setViewMartixArray(array, 6);

		return p;
	}

	void LightingManager::submitLighting(Shader* shader)
	{
		if (!sLightData)
		{
			return;
		}

		if (sLightData->directionalLight)
		{
			sLightData->directionalLight->submit(shader);
		}

		for (auto point_light : sLightData->pointLights)
		{
			point_light->submit(shader);
		}

		for (auto spot_light : sLightData->spotLights)
		{
			spot_light->submit(shader);
		}
	}

	void LightingManager::setLightData(LightData* data)
	{
		sLightData = data;
	}

	void LightingManager::setDirectionalLight(DirectionalLight* agent)
	{
		sLightData->directionalLight = agent;
	}

	void LightingManager::setSkyBox(const std::array<Image*, 6>& images)
	{
		sCubeTextureMap->setImage(images);
		sCubeTextureMap->generate();

		for (uint32 i = 0; i < 6; i++)
		{
			sCubeTextures[i]->setImage(images[i]);
			sCubeTextures[i]->generate();
		}
	}


	TINY_OBJECT_CPP(LightData, TinyObject)
	LightData::LightData()
		: directionalLight(nullptr)
	{

	}

	LightData::~LightData()
	{
		this->directionalLight = nullptr;
		this->pointLights.clear();
		this->spotLights.clear();
	}
}
