#include "GUI.h"
#include "Engine.h"
#include "Statistic.h"
#include "ImGuiHead.h"
#include "GUIWidget.h"

namespace tezcat::Tiny::Core
{
	IMPLEMENT_DELEGATE_INSTANCE(GUI);

	GUI::GUI()
	{
		REGISTER_DELEGATE_INSTANCE(GUI);
	}

	GUI::~GUI()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void GUI::init(Engine* engine)
	{
		const char* glsl_version = "#version 130";
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.Fonts
			->AddFontFromFileTTF(
				"c:/windows/fonts/simhei.ttf",
				16.0f,
				nullptr,
				io.Fonts->GetGlyphRangesChineseFull());
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(engine->getGLFWWindow(), true);
		ImGui_ImplOpenGL3_Init(glsl_version);
	}

	void GUI::render()
	{
		if (!m_InitList.empty())
		{
			for (auto widget : m_InitList)
			{
				widget->init();
			}

			m_InitList.clear();
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		for (auto w : m_Windows)
		{
			w->update();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}




}