#include "MyLightingWindow.h"
#include "MyTextureViewerWindow.h"

CreateInstanceCPP(MyLightingWindow);
MyLightingWindow::MyLightingWindow()
	: GUIWindow("光照管理器(Lighting Manager)")
{

}

MyLightingWindow::~MyLightingWindow()
{
	DeleteInstance();
}


void MyLightingWindow::onRender()
{
	GUIWindow::onRender();

	ImGui::Text("Skybox");
	ImGui::SameLine();
	if (ImGui::Button("Reset"))
	{
		EnvLitMgr::getInstance()->showSkybox();
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(60);
	if (ImGui::DragFloat("Lod", &mSkyboxLod, 0.01f, 0, 4, "%.2f", ImGuiSliderFlags_AlwaysClamp))
	{
		EnvLitMgr::getInstance()->setSkyboxLod(mSkyboxLod);
	}


	mCurrentHDR = EnvLitMgr::getInstance()->getHDRTexture();

	if (mCurrentHDR)
	{
		auto width = mCurrentHDR->getWidth();
		auto height = mCurrentHDR->getHeight();
		auto rate = (float)height / (float)width;
		const int WIDTH = 256;

		ImGui::Image((ImTextureID)mCurrentHDR->getTextureID()
			, ImVec2(WIDTH, WIDTH * rate)
			, ImVec2(0.0f, 1.0f)
			, ImVec2(1.0f, 0.0f));
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
			mCurrentHDR = Texture2D::create();
			mCurrentHDR->setData(img);
			mCurrentHDR->generate();
			EngineEvent::get()->dispatch({ EngineEventID::EE_ChangeEnvLightingTexture, mCurrentHDR });
		}
		ImGui::EndDragDropTarget();
	}

	ImGui::Separator();

	ImGui::Text("EnvCubeMap");
	if (ImGui::Button("IrradianceMap"))
	{
		EnvLitMgr::getInstance()->showIrradianceMap();
	}
	ImGui::SameLine();
	if (ImGui::Button("PrefilterMap"))
	{
		EnvLitMgr::getInstance()->showPrefilterMap();
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
		window->setTexture(TextureMgr::getInstance()->find("CB_BRDF_LUT"));
		window->open(this->getHost());
	}



	ImGui::Separator();
}
