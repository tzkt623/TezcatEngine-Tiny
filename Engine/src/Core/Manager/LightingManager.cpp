/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

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
#include "Core/Data/Resource.h"

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
	bool LightingManager::sIsSceneExited = true;
	float LightingManager::sExposure = 1;
	std::function<void()> LightingManager::sRemovePassFromPipeline = nullptr;

	void LightingManager::init()
	{
		EngineEvent::getInstance()->addListener(EngineEventID::EE_ChangeEnvImage
			, &sIsSceneExited
			, [](const EventData& data)
			{
				setSkyBoxHDRTexture(static_cast<Image*>(data.userData));
				sCalculateEnvLighting = true;
				sMakeCubeTexPass->resetOnceMode();
				sIrradiancePass->resetOnceMode();
				sPrefilterPass->resetOnceMode();
				//sBRDFLUTPass->resetOnceMode();
			});

		EngineEvent::getInstance()->addListener(EngineEventID::EE_OnPushScene
			, &sIsSceneExited
			, [](const EventData& data)
			{
				sIsSceneExited = false;
			});

		EngineEvent::getInstance()->addListener(EngineEventID::EE_OnPopScene
			, &sIsSceneExited
			, [](const EventData& data)
			{
				sIsSceneExited = true;
				sEnableSkyBox = false;
				sCalculateEnvLighting = false;

				if (sTexHDR)
				{
					sTexHDR->deleteObject();
					sTexHDR = nullptr;
				}

				sMakeCubeTexPass->removeFromPipeline();
				sIrradiancePass->removeFromPipeline();
				sPrefilterPass->removeFromPipeline();
				//sBRDFLUTPass->removeFromPipeline();

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
		createHDR2Cube();
		createIrradiance();
		createPrefilter();
		createBRDF_LUT();

		mEnvPassArray.push_back([]()
			{
				sBRDFLUTPass->addToPipeline();
			});
	}

	void LightingManager::close()
	{
		EngineEvent::getInstance()->removeListener(&sIsSceneExited);
	}

	void LightingManager::calculate()
	{
		if (sIsSceneExited)
		{
			return;
		}

		if (!mEnvPassArray.empty())
		{
			for (auto& func : mEnvPassArray)
			{
				func();
			}
			mEnvPassArray.clear();
		}

		//如果启用环境光照,就需要计算环境光
		if (sCalculateEnvLighting)
		{
			sMakeCubeTexPass->addToPipeline();
			sIrradiancePass->addToPipeline();
			sPrefilterPass->addToPipeline();
			//sBRDFLUTPass->addToPipeline();
		}

		//如果启用天空盒
		if (sEnableSkyBox)
		{
			//生成天空盒Pass
			if (sSkyBoxPass == nullptr)
			{
				if (CameraManager::isDataValied())
				{
					auto shader = ShaderManager::find("Unlit/Skybox");
					sSkyBoxPass = ReplacedPipelinePass::create(CameraManager::getMainCamera()->getRenderObserver()
						, shader);
					sSkyBoxPass->saveObject();
					if (sCurrentCubeMap == nullptr)
					{
						sCurrentCubeMap = sCubeTextureMap;
					}

					sSkyBoxPass->setPreFunction([=](ReplacedPipelinePass* pass)
					{
						pass->addCommand<RenderCMD_DrawSkybox>(sSkyboxVertex
							, sCurrentCubeMap
							, sSkyboxLod
							, sCurrentCubeMap->getDataMemFormat().tiny == DataMemFormat::Float
							, sExposure);
					});
				}
			}

			if (sSkyBoxPass)
			{
				//把天空盒加入渲染
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

	void LightingManager::createHDR2Cube()
	{
		int cube_size = sCubeSize;

		auto [flag2, frame_buffer] = FrameBufferManager::create("FB_Cube");
		if (flag2)
		{
			for (uint32 i = 0; i < 6; i++)
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

		auto observer = createObserver();
		observer->setPerspective(90.0f, 0.1f, 10.0f);
		observer->setViewRect(0, 0, cube_size, cube_size);
		observer->setClearOption(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
		observer->setOrderID(-127);

		sMakeCubeTexPass = ReplacedPipelinePass::create(observer, ShaderManager::find("Unlit/EnvMakeCube"));
		sMakeCubeTexPass->setOnceMode();
		sMakeCubeTexPass->setFrameBuffer(frame_buffer);
		sMakeCubeTexPass->setPreFunction([=](ReplacedPipelinePass* pass)
		{
			pass->addCommand<RenderCMD_MakeHDR2Cube>(sHDRVertex, sTexHDR, sCubeTextureMap);
		});
		sMakeCubeTexPass->setOrderID(0);
		sMakeCubeTexPass->saveObject();
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
			pass->addCommand(new RenderCMD_MakeEnvIrradiance(sHDRVertex, sCubeTextureMap, sIrradianceMap));
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
			pass->addCommand(new RenderCMD_MakeEnvPrefilter(sHDRVertex
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
		sBRDFLUTPass->setPreFunction([](ReplacedPipelinePass* pass)
		{
			auto rect_vertex = VertexBufferManager::create("Rect");
			pass->addCommand(new RenderCMD_DrawVertex(rect_vertex));
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
				layout->pushLayoutWithConfig<UniformBufferBinding::SkyBox::ViewIndex>();
				layout->pushLayoutWithConfig<UniformBufferBinding::SkyBox::Roughness>();
				layout->pushLayoutWithConfig<UniformBufferBinding::SkyBox::Resolution>();
			});
		ub->mOnLayoutDataUpdated = [ary](UniformBuffer *buffer)
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

		for (uint32 i = 0; i < 6; i++)
		{
			sCubeTextures[i]->setImage(images[i]);
			sCubeTextures[i]->generate();
		}
	}


	TINY_OBJECT_CPP(LightData, TinyObject);
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
