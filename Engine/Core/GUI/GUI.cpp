#include "GUI.h"
#include "../Engine.h"
#include "../Statistic.h"
#include "../Head//ImGuiHead.h"
#include "Platform/OpenGL/GLHead.h"
#include "GUIWidget.h"

#include "../Platform/OpenGL/WindowsEngine.h"

using namespace tezcat::Tiny::GL;
namespace tezcat::Tiny::Core
{
	GUI::GUI()
		: m_Engine(nullptr)
	{
		SG<GUI>::attach(this);
	}

	GUI::~GUI()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void GUI::init(Engine* engine)
	{
		m_Engine = engine;
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
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigDockingWithShift = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(((WindowsEngine*)engine)->getGLFWWindow(), true);
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

		// 		bool flag = true;
		// 		ImGui::ShowDemoWindow(&flag);

//  		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
//  		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

		for (auto w : mWindows)
		{
			w->update();
		}

		ImGui::Render();

// 		int display_w, display_h;
// 		glfwGetFramebufferSize(window, &display_w, &display_h);
// 		glViewport(0, 0, display_w, display_h);
// 		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
// 		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
}
