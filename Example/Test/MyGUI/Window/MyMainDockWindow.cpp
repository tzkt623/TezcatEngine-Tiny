#include "MyMainDockWindow.h"
#include "../Widget/TransformWidget.h"
#include "MyTextureViewerWindow.h"

MyMainDockWindow::MyMainDockWindow()
	: GUIWindow("TestWindow")
	, mWindowFlags(ImGuiWindowFlags_MenuBar
		| ImGuiWindowFlags_NoDocking
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoBringToFrontOnFocus
		| ImGuiWindowFlags_NoNavFocus
		| ImGuiWindowFlags_NoBackground)
	, mDockspaceFlags(ImGuiDockNodeFlags_None)
{

}

MyMainDockWindow::~MyMainDockWindow()
{

}

void MyMainDockWindow::onUpdate()
{



}

bool MyMainDockWindow::begin()
{
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, 5.0f);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	return ImGui::Begin("Test", 0, mWindowFlags);
}

void MyMainDockWindow::end()
{
	ImGui::End();
}
void MyMainDockWindow::onRender()
{
	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), mDockspaceFlags);
	}
	else
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("ERROR: Docking is not enabled! See Demo > Configuration.");
		ImGui::Text("Set io.ConfigFlags |= ImGuiConfigFlags_DockingEnable in your code, or ");
		ImGui::SameLine(0.0f, 0.0f);
		if (ImGui::SmallButton("click here"))
		{
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		}
	}

	//-----------------------------------------
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Scene"))
		{
			if (ImGui::MenuItem("Close Current Scene"))
			{
				EngineEvent::get()->dispatch(EventData{ EngineEventID::EE_PopScene });
			}

			ImGui::Separator();

			auto& scenes = SceneMgr::getInstance()->getAllScenes();
			for (auto it : scenes)
			{
				if (ImGui::MenuItem(it.first.c_str()))
				{
					EngineEvent::get()->dispatch(EventData{ EngineEventID::EE_PopScene });
					EngineEvent::get()->dispatch(EventData{ EngineEventID::EE_PushScene, it.second });
				}
			}

			ImGui::EndMenu();
		}

		//-----------------------------------------
		if (ImGui::BeginMenu("Shader"))
		{
			if (ImGui::BeginMenu("Rebuild"))
			{
				if (ImGui::MenuItem("All"))
				{
					ShaderMgr::getInstance()->rebuildShaders();
				}
				ImGui::Separator();

				auto& packages = ShaderMgr::getInstance()->getAllShaderPackages();
				for (auto package : packages)
				{
					if (ImGui::MenuItem(package->getName().c_str()))
					{
						package->rebuild();
					}
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		//-----------------------------------------
		if (ImGui::BeginMenu("Debug"))
		{
			if (ImGui::MenuItem("ShadowTexture"))
			{
				auto window = new MyTextureViewerWindow("ShadowTexture");
				window->setTexture(TextureMgr::getInstance()->findTexture("Shadow"));
				window->open(this->getHost());
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Skybox"))
			{
				EnvLighting::getInstance()->showSkybox();
			}

			if (ImGui::MenuItem("IrradianceMap"))
			{
				EnvLighting::getInstance()->showIrradianceMap();
			}

			if (ImGui::MenuItem("PrefilterMap"))
			{
				EnvLighting::getInstance()->showPrefilterMap();
			}

			if (ImGui::MenuItem("BRDF LUT"))
			{
				auto window = new MyTextureViewerWindow("BRDF LUT");
				window->setTexture(TextureMgr::getInstance()->findTexture("CB_BRDF_LUT"));
				window->open(this->getHost());
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}
