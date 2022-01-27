#include "GUI.h"
#include "Engine.h"
#include "Statistic.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "CameraManager.h"
#include "Transform.h"
#include "Config.h"

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

	void GUI::createGUI()
	{
		// 		auto window = new GUIWindow(u8"统计信息");
		// 		auto text = new GUIText(u8"FPS");
		// 		text->function = []()
		// 		{
		// 			return std::string(U8("FPS: %.1f(%.3f ms/frame)"), ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate).c_str();
		// 		};
		// 
		// 		m_ObjectWindow = new GUIWindow(u8"对象");
		// 		auto position = new GUIDragFloat3(u8"坐标(x,y,z)");
		// 		position->preFunction = [](float* data)
		// 		{
		// 			auto camera = Engine::getInstance()->getCameraManager()->getCamera("Main");
		// 			auto position = camera->getTransform()->getPosition();
		// 			data[0] = position.x;
		// 			data[1] = position.y;
		// 			data[2] = position.z;
		// 		};
		// 
		// 		position->postFunction = [](float* data)
		// 		{
		// 			auto camera = Engine::getInstance()->getCameraManager()->getCamera("Main");
		// 			camera->getTransform()->setPosition(glm::vec3(data[0], data[1], data[2]));
		// 		};
		// 
		// 		m_ObjectWindow->addChild(position);
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
		// 		static bool flag = true;
		// 		ImGui::ShowDemoWindow(&flag);
// 		ImGui::Begin(Tools::fromU8(u8"统计信息").c_str());
// 		ImGui::Text(Tools::fromU8(u8"FPS: %.1f(%.3f ms/frame)").c_str(), ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
// 		ImGui::Text(Tools::fromU8(u8"PassCount: %d").c_str(), Statistic::PassCount);
// 		ImGui::End();

		/*
		ImGui::Begin(U8("场景"));
		static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		ImGuiTreeNodeFlags node_flags = base_flags;
		node_flags |= ImGuiTreeNodeFlags_Selected;
		auto scene = Engine::getInstance()->getSceneManager()->getCurrentScene();
		scene->foreachChild(
			[&]()
			{
				node_flags = base_flags;
			},
			[&](Module::GameObject* go)
			{
				ImGui::TreeNodeEx(go->getName().c_str(), node_flags);
				if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
				{
					go;
				}
			},
				[this]()
			{
				ImGui::TreePop();
			},
				[this]()
			{

			});
		ImGui::End();
		*/


		// 		ImGui::Begin(U8("对象"));
		// 		static glm::vec3 position(0.0f);
		// 		auto camera = Engine::getInstance()->getCameraManager()->getCamera("Main");
		// 		position = camera->getTransform()->getPosition();
		// 		ImGui::DragFloat3(U8("坐标"), glm::value_ptr(position), 0.1f);
		// 		camera->getTransform()->setPosition(position);
		// 		ImGui::End();
		//m_ObjectWindow->render();

		for (auto w : m_Windows)
		{
			w->update();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}



	//----------------------------------------------------------------------
	GUIWidget::GUIWidget(const std::u8string& name) :
		m_Name(Tools::fromU8(name))
	{

	}

	GUIWidget::GUIWidget(const std::string& name) :
		m_Name(name)
	{

	}

	GUIWidget::GUIWidget(const char* name) :
		m_Name(name)
	{

	}

	GUIWidget::~GUIWidget()
	{

	}

	void GUIWidget::update()
	{
		this->onUpdate();
		this->onRender();
	}


	//-------------------------------------------------------
	void GUIParentWidget::onRender()
	{

	}

	void GUIParentWidget::update()
	{
		this->onUpdate();
		this->onRender();

		this->beginParent();
		for (auto w : m_Children)
		{
			w->update();
		}
		this->endParent();
	}


	//-------------------------------------------------------
	GUIWindow::GUIWindow(const std::u8string& name) :
		GUIParentWidget(name)
	{

	}

	GUIWindow::GUIWindow(const std::string& name) :
		GUIParentWidget(name)
	{

	}

	GUIWindow::GUIWindow(const char* name) :
		GUIParentWidget(name)
	{

	}

	GUIWindow::~GUIWindow()
	{

	}

	void GUIWindow::beginParent()
	{
		ImGui::Begin(this->getName());
	}

	void GUIWindow::endParent()
	{
		ImGui::End();
	}

	void GUIWindow::open()
	{
		GUI::getInstance()->addWindow(this);
	}

	void GUIWindow::close()
	{
		GUI::getInstance()->removeWindow(this);
	}


	//-------------------------------------------------------------------------
	GUIDragFloat3::GUIDragFloat3(const std::u8string& name) :
		GUIWidget(name),
		m_Data(),
		m_Speed(0.1f),
		m_Min(0.0f),
		m_Max(0.0f)
	{

	}

	GUIDragFloat3::~GUIDragFloat3()
	{
		postFunction = nullptr;
	}

	void GUIDragFloat3::onRender()
	{
		ImGui::DragFloat3(this->getName(), m_Data, m_Speed, m_Min, m_Max);
		postFunction(m_Data);
	}


	//------------------------------

	GUIText::GUIText(const std::u8string& name) :
		GUIWidget(name)
	{

	}

	GUIText::~GUIText()
	{

	}


	void GUIText::onRender()
	{
		ImGui::Text(m_Data.c_str());
	}
}