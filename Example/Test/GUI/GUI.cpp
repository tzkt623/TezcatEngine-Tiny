#include "GUI.h"
#include "GUIWidget.h"

namespace tezcat::Editor
{
	GUI::GUI()
	{

	}

	GUI::~GUI()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void GUI::init(GLFWwindow* window)
	{
		const char* glsl_version = "#version 410 core";
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.Fonts->AddFontFromFileTTF(
			"c:/windows/fonts/simhei.ttf",
			16.0f,
			nullptr,
			io.Fonts->GetGlyphRangesChineseFull());
		// 		io.Fonts->AddFontFromFileTTF(
		// 			"c:/windows/fonts/simhei.ttf",
		// 			16.0f,
		// 			nullptr,
		// 			io.Fonts->GetGlyphRangesChineseFull());
				//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
				//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigDockingWithShift = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
	}

	void GUI::render()
	{
		if (!mInitList.empty())
		{
			for (auto widget : mInitList)
			{
				widget->init();
			}

			mInitList.clear();
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		auto it = mWindows.begin();

		while (it != mWindows.end())
		{
			if ((*it)->isClosed())
			{
				delete (*it);
				it = mWindows.erase(it);
			}
			else
			{
				(*it)->update();
				++it;
			}
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		TINY_GL_CHECK("ImGUI");

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
}
