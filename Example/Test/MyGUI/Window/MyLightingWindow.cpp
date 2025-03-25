#include "MyLightingWindow.h"
#include "MyTextureViewerWindow.h"

namespace tezcat::Editor
{
	TINY_EDITOR_WINDOW_INSTANCE_CPP(MyLightingWindow)

		MyLightingWindow::MyLightingWindow()
		: GUIWindow("光照管理器(Lighting Manager)")
		, mCurrentHDR(nullptr)
		, mSkyboxLod(0)
	{
		EngineEvent::getInstance()->addListener(EngineEventID::EE_PopScene, this
			, [](const EventData& data)
			{

			});
	}

	MyLightingWindow::~MyLightingWindow()
	{
		TINY_EDITOR_WINDOW_DELETE_INSTACNE();
	}


	void MyLightingWindow::onRender()
	{
		GUIWindow::onRender();

		ImGui::Text("Skybox");
		ImGui::SameLine();
		if (ImGui::Button("Reset"))
		{
			LightingManager::showSkybox();
		}
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60);
		if (ImGui::DragFloat("Lod", &mSkyboxLod, 0.01f, 0, 4, "%.2f", ImGuiSliderFlags_AlwaysClamp))
		{
			LightingManager::setSkyboxLod(mSkyboxLod);
		}

		auto exp = LightingManager::getExposure();
		ImGui::DragFloat("Exposure", &exp, 0.01f);
		LightingManager::setExposure(exp);


		mCurrentHDR = LightingManager::getSkyBoxHDRTexture();

		if (mCurrentHDR)
		{
			auto width = mCurrentHDR->getWidth();
			auto height = mCurrentHDR->getHeight();
			auto rate = (float)height / (float)width;
			const float WIDTH = 256.0f;

			ImGui::Image((ImTextureID)mCurrentHDR->getTextureID()
				, ImVec2(WIDTH, WIDTH * rate)
				, MyGUIContext::UV0
				, MyGUIContext::UV1);
		}
		else
		{
			ImGui::Image((ImTextureID)0, ImVec2(256, 256));
		}

		ImGuiHelper::dropResource([this](file_path path)
		{
			Image* img = Image::create();
			img->openFile(path, true);

			mCurrentHDR = nullptr;
			EngineEvent::getInstance()->dispatch({ EngineEventID::EE_ChangeEnvImage, img });
		});

		//if (ImGui::BeginDragDropTarget())
		//{
		//	auto [flag, drop_name] = MyGUIContext::DragDropController.dropData();
		//	if (flag)
		//	{
		//		auto payload = ImGui::AcceptDragDropPayload(drop_name.data());
		//		if (payload)
		//		{
		//			Image* img = Image::create();
		//			img->openFile(MyGUIContext::DragDropController.getFilePath(), true);
		//
		//			mCurrentHDR = nullptr;
		//			EngineEvent::getInstance()->dispatch({ EngineEventID::EE_ChangeEnvImage, img });
		//		}
		//	}
		//
		//	ImGui::EndDragDropTarget();
		//}

		ImGui::Separator();

		ImGui::Text("EnvCubeMap");
		if (ImGui::Button("IrradianceMap"))
		{
			LightingManager::showIrradianceMap();
		}
		ImGui::SameLine();
		if (ImGui::Button("PrefilterMap"))
		{
			LightingManager::showPrefilterMap();
		}

		ImGui::Separator();

		/*
		ImGui::Text("Textures");
		ImGui::PushID(0);
		if (ImGui::BeginListBox(""))
		{
			auto& list = ShadowCasterMgr::getInstance()->getAllCaster();
			for (auto& caster : list)
			{
				ImGui::Selectable("Shadow");
				if (ImGui::IsItemClicked())
				{
					auto window = new MyTextureViewerWindow("ShadowTexture");
					window->setTexture(caster->getShadwowTexutre());
					window->open(this->getHost());
				}
			}

			ImGui::EndListBox();
		}
		ImGui::PopID();
		*/

		if (ImGui::Button("BRDF LUT"))
		{
			auto window = new MyTextureViewerWindow("BRDF LUT");
			window->setTexture(TextureManager::find("CB_BRDF_LUT"));
			window->open(this->getHost());
		}

		ImGui::Separator();


		static std::function<void(ImVec2&, Texture*, const char*)> func =
			[](ImVec2& size, Texture* tex, const char* name)
			{
				ImGui::BeginGroup();
				ImGui::Image((ImTextureID)tex->getTextureID()
					, size
					, MyGUIContext::UV0, MyGUIContext::UV1);
				ImGui::Text(name);
				ImGui::EndGroup();
			};


		auto cube_array = LightingManager::getCubeMapTextureArray();
		if (cube_array)
		{
			ImVec2 size(128, 128);
			//X
			func(size, cube_array[0], "X+");
			ImGui::SameLine();
			func(size, cube_array[1], "X-");

			//Y
			func(size, cube_array[2], "Y+");
			ImGui::SameLine();
			func(size, cube_array[3], "Y-");

			//
			func(size, cube_array[4], "Z+");
			ImGui::SameLine();
			func(size, cube_array[5], "Z-");
		}
	}
}
