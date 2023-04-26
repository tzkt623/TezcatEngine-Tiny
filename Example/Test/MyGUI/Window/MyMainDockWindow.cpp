#include "MyMainDockWindow.h"
#include "../Widget/TransformWidget.h"


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

void MyMainDockWindow::onRender()
{
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, 5.0f);

	bool p_open = true;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	if (ImGui::Begin("Test", &p_open, mWindowFlags))
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

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Scene"))
			{
				if (ImGui::MenuItem("Close Current Scene"))
				{
					SceneMgr::getInstance()->popScene();
				}

				ImGui::Separator();

				auto& scenes = SceneMgr::getInstance()->getAllScenes();
				for (auto it : scenes)
				{
					if (ImGui::MenuItem(it.first.c_str()))
					{
						SceneMgr::getInstance()->switchScene(it.second);
					}
				}

				ImGui::EndMenu();
			}

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

			ImGui::EndMenuBar();
		}

		ImGui::End();
	}

}

void MyMainDockWindow::begin()
{

}

void MyMainDockWindow::end()
{

}

