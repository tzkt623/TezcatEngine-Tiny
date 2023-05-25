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

	ImGui::Text("Textures");
	if (ImGui::Button("ShadowTexture"))
	{
		auto window = new MyTextureViewerWindow("ShadowTexture");
		window->setTexture(TextureMgr::getInstance()->findTexture("Shadow"));
		window->open(this->getHost());
	}

	ImGui::SameLine();

	if (ImGui::Button("BRDF LUT"))
	{
		auto window = new MyTextureViewerWindow("BRDF LUT");
		window->setTexture(TextureMgr::getInstance()->findTexture("CB_BRDF_LUT"));
		window->open(this->getHost());
	}

	if (mHDR.empty())
	{
		TextureMgr::getInstance()->outputAll2DHDR(mHDR);
	}

	if (ImGui::BeginListBox("HDR Maps"))
	{
		for (auto tex : mHDR)
		{
			if (ImGui::Selectable(tex->getName().c_str(), mCurrentHDR == tex))
			{
				mCurrentHDR = tex;
				EngineEvent::get()->dispatch({ EngineEventID::EE_RenderEnv, (void*)&tex->getName() });
			}
		}
		ImGui::EndListBox();
	}
	ImGui::Separator();
}
