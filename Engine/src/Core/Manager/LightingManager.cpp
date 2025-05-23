﻿/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Core/Manager/LightingManager.h"
#include "Core/Manager/FrameBufferManager.h"
#include "Core/Manager/TextureManager.h"
#include "Core/Manager/VertexBufferManager.h"
#include "Core/Manager/ShaderManager.h"
#include "Core/Manager/CameraManager.h"
#include "Core/Manager/ResourceManager.h"

#include "Core/Component/Transform.h"
#include "Core/Component/Light.h"
#include "Core/Component/Camera.h"

#include "Core/Renderer/RenderObserver.h"
#include "Core/Renderer/BaseGraphics.h"
#include "Core/Renderer/Texture.h"
#include "Core/Renderer/RenderCommand.h"
#include "Core/Renderer/Vertex.h"
#include "Core/Renderer/VertexBuffer.h"
#include "Core/Renderer/FrameBuffer.h"
#include "Core/Renderer/Pipeline.h"
#include "Core/Renderer/Material.h"

#include "Core/Shader/Shader.h"
#include "Core/Shader/ShaderPackage.h"

#include "Core/Event/EngineEvent.h"

#include "Core/Data/Image.h"
#include "Core/Manager/SceneManager.h"

namespace tezcat::Tiny
{
	ReplacedPipelinePass* LightingManager::sBRDFLUTPass = nullptr;
	ReplacedPipelinePass* LightingManager::sPrefilterPass = nullptr;
	ReplacedPipelinePass* LightingManager::sIrradiancePass = nullptr;
	ReplacedPipelinePass* LightingManager::sMakeCubeTexPass = nullptr;
	ReplacedPipelinePass* LightingManager::sSkyBoxPass = nullptr;
	std::vector<std::function<void()>> LightingManager::mEnvPassArray;

	Texture2D* LightingManager::sTexHDR = nullptr;
	Texture2D* LightingManager::sBRDFLUTMap = nullptr;
	TextureCube* LightingManager::sCubeTextureMap = nullptr;
	TextureCube* LightingManager::sPrefilterMap = nullptr;
	TextureCube* LightingManager::sIrradianceMap = nullptr;
	TextureCube* LightingManager::sCurrentCubeMap = nullptr;
	std::array<Texture2D*, 6> LightingManager::sCubeTextures{ nullptr };
	Vertex* LightingManager::sSkyboxVertex = nullptr;
	Vertex* LightingManager::sHDRVertex = nullptr;
	LightData* LightingManager::sLightData = nullptr;

	RenderObserverMultView* LightingManager::sObserverHDR = nullptr;
	RenderObserverMultView* LightingManager::sObserverIrradiance = nullptr;
	RenderObserverMultView* LightingManager::sObserverPrefilter = nullptr;
	RenderObserver* LightingManager::sObserverBRDF_LUT = nullptr;



	bool LightingManager::sCloseEnvLighting = true;
	float LightingManager::sSkyboxLod = 0;
	int32_t LightingManager::sCubeSize = 1024;
	int32_t LightingManager::sPrefilterSize = 128;
	int32_t LightingManager::sIrrSize = 32;
	bool LightingManager::sEnableSkyBox = false;
	bool LightingManager::sIsSceneExited = true;
	bool LightingManager::sRefreshSkyBox = false;

	float LightingManager::sExposure = 1;
	std::function<void()> LightingManager::sRemovePassFromPipeline = nullptr;

	void LightingManager::init()
	{
		EngineEvent::getInstance()->addListener(EngineEventID::EE_ChangeEnvImage
			, &sIsSceneExited
			, [](const EventData& data)
			{
				setSkyBoxHDRTexture(static_cast<Image*>(data.userData));
				sRefreshSkyBox = true;
				//sMakeCubeTexPass->resetOnceMode();
				//sIrradiancePass->resetOnceMode();
				//sPrefilterPass->resetOnceMode();
				//sBRDFLUTPass->resetOnceMode();
			});

		EngineEvent::getInstance()->addListener(EngineEventID::EE_BeforeSceneEnter
			, &sIsSceneExited
			, [](const EventData& data)
			{
				sIsSceneExited = false;
			});

		EngineEvent::getInstance()->addListener(EngineEventID::EE_BeforeSceneExit
			, &sIsSceneExited
			, [](const EventData& data)
			{
				sIsSceneExited = true;
				sEnableSkyBox = false;
				sRefreshSkyBox = false;

				if (sTexHDR)
				{
					sTexHDR->deleteObject();
					sTexHDR = nullptr;
				}

				//sMakeCubeTexPass->removeFromPipeline();
				//sIrradiancePass->removeFromPipeline();
				//sPrefilterPass->removeFromPipeline();
				//sBRDFLUTPass->removeFromPipeline();

				if (sSkyBoxPass)
				{
					sSkyBoxPass->removeFromPipeline();
					sSkyBoxPass->deleteObject();
					sSkyBoxPass = nullptr;
				}

				sCurrentCubeMap = nullptr;
			});

		EngineEvent::getInstance()->addListener(EngineEventID::EE_SetMainCamera
			, &sIsSceneExited
			, [](const EventData& data)
			{
				if (sSkyBoxPass)
				{
					sSkyBoxPass->removeFromPipeline();
					sSkyBoxPass->deleteObject();
					sSkyBoxPass = nullptr;
				}
			});

		sSkyboxVertex = VertexBufferManager::create("Skybox");
		sHDRVertex = VertexBufferManager::create("Cube");

		sObserverHDR = createObserver();
		sObserverHDR->saveObject();
		sObserverHDR->setPerspective(90.0f, 0.1f, 10.0f);
		sObserverHDR->setClearOption(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
		sObserverHDR->setSortingID(65535);

		createHDR2Cube();
		createIrradiance();
		createPrefilter();
		createBRDF_LUT();
	}

	void LightingManager::close()
	{
		EngineEvent::getInstance()->removeListener(&sIsSceneExited);
	}

	void LightingManager::preRender()
	{
		if (SceneManager::isEmpty())
		{
			return;
		}

		//如果启用天空盒
		if (sEnableSkyBox)
		{
			//生成天空盒Pass
			if (sSkyBoxPass == nullptr && CameraManager::isDataValied())
			{
				if (auto camera = CameraManager::getMainCamera().lock())
				{
					if ((camera->getClearOption() & ClearOption::CO_Skybox) == ClearOption::CO_Skybox)
					{
						auto shader = ShaderManager::find("Unlit/Skybox");
						sSkyBoxPass = ReplacedPipelinePass::create(camera->getRenderObserver()
							, shader);
						sSkyBoxPass->saveObject();
						if (sCurrentCubeMap == nullptr)
						{
							sCurrentCubeMap = sCubeTextureMap;
						}

						sSkyBoxPass->setCustomCulling([=](ReplacedPipelinePass* pass)
						{
							pass->addCommand<RenderCMD_DrawSkybox>(sSkyboxVertex
								, sCurrentCubeMap
								, sSkyboxLod
								, sCurrentCubeMap->getDataMemFormat().tiny == DataMemFormat::Float
								, sExposure);
						});
					}
				}
			}

			if (sSkyBoxPass)
			{
				if (auto camera = CameraManager::getMainCamera().lock())
				{
					if ((camera->getClearOption() & ClearOption::CO_Skybox) != ClearOption::CO_Skybox)
					{
						sSkyBoxPass->removeFromPipeline();
						return;
					}


					if (sRefreshSkyBox)
					{
						sRefreshSkyBox = false;
						sMakeCubeTexPass->resetOnceMode();
						sIrradiancePass->resetOnceMode();
						sPrefilterPass->resetOnceMode();
						//sBRDFLUTPass->resetOnceMode();

						sMakeCubeTexPass->addToPipeline();
						sIrradiancePass->addToPipeline();
						sPrefilterPass->addToPipeline();
						sBRDFLUTPass->addToPipeline();
					}

					//把天空盒加入渲染
					//replacepass还有一个save在manager中
					sSkyBoxPass->addToPipeline();
				}
			}
		}
		else
		{
			if (sSkyBoxPass)
			{
				sSkyBoxPass->removeFromPipeline();
			}
		}


		//if (!mEnvPassArray.empty())
		//{
		//	for (auto& func : mEnvPassArray)
		//	{
		//		func();
		//	}
		//	mEnvPassArray.clear();
		//}
	}

	void LightingManager::submitEnvLighting(Shader* shader)
	{
		Graphics::getInstance()->setGlobalTextureCube(shader, ShaderParam::IBL::TexIrradiance, sIrradianceMap);
		Graphics::getInstance()->setGlobalTextureCube(shader, ShaderParam::IBL::TexPrefilter, sPrefilterMap);
		Graphics::getInstance()->setGlobalTexture2D(shader, ShaderParam::IBL::TexBRDFLUT, sBRDFLUTMap);
	}

	void LightingManager::createHDR2Cube()
	{
		int cube_size = sCubeSize;
		sObserverHDR->setViewRect(0, 0, cube_size, cube_size);

		auto [flag2, frame_buffer] = FrameBufferManager::create("FB_Cube");
		if (flag2 == FlagCreate::Succeeded)
		{
			for (uint32_t i = 0; i < 6; i++)
			{
				sCubeTextures[i] = Texture2D::create(std::format("CubeTexture{}", i));
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

		sMakeCubeTexPass = ReplacedPipelinePass::create(sObserverHDR, ShaderManager::find("Hide/EnvMakeCube"));
		sMakeCubeTexPass->setOnceMode();
		sMakeCubeTexPass->setFrameBuffer(frame_buffer);
		sMakeCubeTexPass->setCustomCulling([=](ReplacedPipelinePass* pass)
		{
			pass->addCommand<RenderCMD_MakeHDR2Cube>(sHDRVertex, sTexHDR, sCubeTextureMap, std::array<int32_t, 2>{ cube_size, cube_size });
		});
		sMakeCubeTexPass->setUserSortingID(0);
		sMakeCubeTexPass->saveObject();
	}

	void LightingManager::createIrradiance()
	{
		const auto irr_size = sIrrSize;

		auto [flag1, frame_buffer] = FrameBufferManager::create("FB_Irradiance");
		if (flag1 == FlagCreate::Succeeded)
		{
			auto texture = TextureCube::create("CB_IrradianceMap");
			texture->setConfig(irr_size
				, TextureInternalFormat::RGB16F
				, TextureFormat::RGB
				, DataMemFormat::Float
				, TextureFilter::Linear_Mipmap_Linear
				, TextureFilter::Linear);
			texture->setAttachPosition(TextureAttachPosition::ColorComponent);
			sIrradianceMap = texture;

			frame_buffer->addAttachment(texture);
			frame_buffer->generate();
		}

		//sObserverIrradiance = createObserver();
		//sObserverIrradiance->saveObject();
		//sObserverIrradiance->setPerspective(90.0f, 0.1f, 10.0f);
		//sObserverIrradiance->setViewRect(0, 0, irr_size, irr_size);
		//sObserverIrradiance->setClearOption(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
		//sObserverIrradiance->setSortingID(65534);

		auto shader = ShaderManager::find("Hide/EnvMakeIrradiance");

		sIrradiancePass = ReplacedPipelinePass::create(sObserverHDR, shader);
		sIrradiancePass->setOnceMode();
		sIrradiancePass->setFrameBuffer(frame_buffer);
		sIrradiancePass->setCustomCulling([=](ReplacedPipelinePass* pass)
		{
			pass->addCommand(new RenderCMD_MakeEnvIrradiance(sHDRVertex, sCubeTextureMap, sIrradianceMap, std::array<int32_t, 2>{irr_size, irr_size}));
		});
		sIrradiancePass->setUserSortingID(1);
		sIrradiancePass->saveObject();
	}

	void LightingManager::createPrefilter()
	{
		const int prefilter_size = sPrefilterSize;

		auto [flag1, frame_buffer] = FrameBufferManager::create("FB_Prefilter");
		if (flag1 == FlagCreate::Succeeded)
		{
			auto texture = TextureCube::create("CB_PrefilterMap");

			texture->setConfig(prefilter_size
				, TextureInternalFormat::RGB16F
				, TextureFormat::RGB
				, DataMemFormat::Float
				, TextureFilter::Linear_Mipmap_Linear
				, TextureFilter::Linear);

			texture->setAttachPosition(TextureAttachPosition::ColorComponent);
			sPrefilterMap = texture;

			frame_buffer->addAttachment(texture);
			frame_buffer->generate();
		}

		//sObserverPrefilter = createObserver();
		//sObserverPrefilter->saveObject();
		//sObserverPrefilter->setPerspective(90.0f, 0.1f, 10.0f);
		//sObserverPrefilter->setViewRect(0, 0, prefilter_size, prefilter_size);
		//sObserverPrefilter->setClearOption(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
		//sObserverPrefilter->setSortingID(65533);

		sPrefilterPass = ReplacedPipelinePass::create(sObserverHDR
			, ShaderManager::find("Hide/EnvMakePrefilter"));
		sPrefilterPass->setOnceMode();
		sPrefilterPass->setFrameBuffer(frame_buffer);
		sPrefilterPass->setCustomCulling([=](ReplacedPipelinePass* pass)
		{
			pass->addCommand(new RenderCMD_MakeEnvPrefilter(sHDRVertex
				, sCubeTextureMap
				, sPrefilterMap
				, 5
				, prefilter_size
				, prefilter_size
				, sCubeSize));
		});
		sPrefilterPass->setUserSortingID(2);
		sPrefilterPass->saveObject();
	}

	void LightingManager::createBRDF_LUT()
	{
		auto [flag1, frame_buffer] = FrameBufferManager::create("FB_BRDF_LUT");
		if (flag1 == FlagCreate::Succeeded)
		{
			auto texture = Texture2D::create("CB_BRDF_LUT");

			texture->setConfig(sCubeSize, sCubeSize
				, TextureInternalFormat::RG16F
				, TextureFormat::RG
				, DataMemFormat::Float);
			texture->setAttachPosition(TextureAttachPosition::ColorComponent);
			sBRDFLUTMap = texture;


			frame_buffer->addAttachment(texture);
			frame_buffer->generate();
		}

		sObserverBRDF_LUT = RenderObserver::create();
		sObserverBRDF_LUT->saveObject();
		sObserverBRDF_LUT->setPerspective(90.0f, 0.1f, 10.0f);
		sObserverBRDF_LUT->setViewRect(0, 0, sCubeSize, sCubeSize);
		sObserverBRDF_LUT->setClearOption(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
		sObserverBRDF_LUT->setSortingID(65532);

		sBRDFLUTPass = ReplacedPipelinePass::create(sObserverBRDF_LUT
			, ShaderManager::find("Hide/EnvMakeBRDFLut"));
		sBRDFLUTPass->setOnceMode();
		sBRDFLUTPass->setFrameBuffer(frame_buffer);
		sBRDFLUTPass->setCustomCulling([](ReplacedPipelinePass* pass)
		{
			auto rect_vertex = VertexBufferManager::create("Rect");
			pass->addCommand(new RenderCMD_DrawVertex(rect_vertex));
		});
		sBRDFLUTPass->setUserSortingID(3);
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
		float4x4 ary[6]
		{
			glm::lookAt(float3(0.0f, 0.0f, 0.0f), float3(1.0f,  0.0f,  0.0f), float3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(float3(0.0f, 0.0f, 0.0f), float3(-1.0f, 0.0f,  0.0f), float3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(float3(0.0f, 0.0f, 0.0f), float3(0.0f,  1.0f,  0.0f), float3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(float3(0.0f, 0.0f, 0.0f), float3(0.0f, -1.0f,  0.0f), float3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(float3(0.0f, 0.0f, 0.0f), float3(0.0f,  0.0f,  1.0f), float3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(float3(0.0f, 0.0f, 0.0f), float3(0.0f,  0.0f, -1.0f), float3(0.0f, -1.0f,  0.0f))
		};

		RenderObserverMultView* p = RenderObserverMultView::create();
		p->createUniformBuffer();
		auto ub = p->getUniformBuffer();
		ub->setLayout<UniformBufferBinding::SkyBox>([](UniformBufferLayout* layout)
			{
				layout->pushLayoutWithConfig<UniformBufferBinding::SkyBox::MatrixP>();
				layout->pushLayoutWithConfig<UniformBufferBinding::SkyBox::MatrixV6>();
				layout->pushLayoutWithConfig<UniformBufferBinding::SkyBox::Roughness>();
				layout->pushLayoutWithConfig<UniformBufferBinding::SkyBox::Resolution>();
			});
		ub->mOnLayoutDataUpdated = [ary](UniformBuffer* buffer)
			{
				buffer->updateWithConfig<UniformBufferBinding::SkyBox::MatrixV6>(ary);
			};

		ub->updateWithConfig<UniformBufferBinding::SkyBox::MatrixV6>(ary);

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

		for (uint32_t i = 0; i < 6; i++)
		{
			sCubeTextures[i]->setImage(images[i]);
			sCubeTextures[i]->generate();
		}
	}

	void LightingManager::loadSkyboxHDR(const file_path& path)
	{
		auto img = ResourceManager::loadOnly<Image>(path);
		setSkyBoxHDRTexture(img);
		sEnableSkyBox = true;
		sRefreshSkyBox = true;
	}

	TINY_OBJECT_CPP(LightData, TinyObject);
	LightData::LightData()
		: directionalLight(nullptr)
	{

	}

	LightData::~LightData()
	{

	}

	void LightData::onClose()
	{
		this->directionalLight = nullptr;
		this->pointLights.clear();
		this->spotLights.clear();
	}
}
