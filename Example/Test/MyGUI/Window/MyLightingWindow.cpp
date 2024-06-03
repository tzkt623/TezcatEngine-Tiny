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
			const int WIDTH = 256;

			ImGui::Image((ImTextureID)mCurrentHDR->getTextureID()
				, ImVec2(WIDTH, WIDTH * rate)
				, MyGUIContext::UV0
				, MyGUIContext::UV1);
		}
		else
		{
			ImGui::Image((ImTextureID)0, ImVec2(256, 256));
		}

		if (ImGui::BeginDragDropTarget())
		{
			auto payload = ImGui::AcceptDragDropPayload("Drag Image");
			if (payload)
			{
				char* path = new char[payload->DataSize + 1];
				memcpy_s(path, payload->DataSize, payload->Data, payload->DataSize);
				path[payload->DataSize] = '\0';

				Image* img = Image::create();
				img->openFile(path, true);
				delete[] path;
				mCurrentHDR = nullptr;
				EngineEvent::get()->dispatch({ EngineEventID::EE_ChangeEnvImage, img });
			}
			ImGui::EndDragDropTarget();
		}

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
