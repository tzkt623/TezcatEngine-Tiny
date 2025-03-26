#include "MyMainWindow.h"
#include "MyTextureViewerWindow.h"
#include "MyResourceWindow.h"
#include "MyShaderEditorWindow.h"
#include "MyObjectInfoWindow.h"
#include "MyOverviewWindow.h"
#include "MyGCInfoWindow.h"
#include "MyLogWindow.h"
#include "MySceneWindow.h"
#include "MyLightingWindow.h"
#include "MyFrameBufferViewerWindow.h"

#include "../../MyEvent.h"
namespace tezcat::Editor
{
	MyMainWindow::MyMainWindow()
		: GUIWindow("MainWindow")
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
		MyEvent::get()->addListener(MyEventID::Window_OpenShaderEditor, this, [this](const EventData& data)
		{
			auto path = (std::filesystem::path*)data.userData;
			MyShaderEditorWindow::create(*path, this->getHost())->setFocus();
		});

		MyEvent::get()->addListener(MyEventID::Window_OpenImageViwer, this, [this](const EventData& data)
		{
			auto path = (std::filesystem::path*)data.userData;
			auto window = new MyTextureViewerWindow(path->filename().string());
			window->loadTexture(*path);
			window->open(this->getHost());
		});

		EngineEvent::getInstance()->addListener(EngineEventID::EE_BeforeSceneExit, this,
		[](const EventData& data)
		{
			MyGUIContext::clearOnPopScene();
		});
	}

	MyMainWindow::~MyMainWindow()
	{

	}

	void MyMainWindow::init()
	{
		MyObjectInfoWindow::create(this->getHost());
		MyOverviewWindow::create(this->getHost());
		MySceneWindow::create(this->getHost());
		MyResourceWindow::create(this->getHost());
		MyLogWindow::create(this->getHost());
		MyLightingWindow::create(this->getHost());
		//MyGCInfoWindow::create(this->getHost());
	}

	bool MyMainWindow::begin()
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

	void MyMainWindow::end()
	{
		ImGui::End();
	}
	void MyMainWindow::onRender()
	{
		GUIWindow::onRender();

		//ImGui::PopStyleVar();
		ImGui::PopStyleVar(3);

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
			if (ImGui::BeginMenu("文件(File)"))
			{
				if (ImGui::MenuItem("新场景(NewScene)"))
				{
					SceneManager::switchScene(Scene::create("NewScene"));
				}

				if (SceneManager::empty())
				{
					ImGui::TextColored(ImVec4(0.2f, 0.2f, 0.2f, 1.0f), "关闭当前场景(Close Current Scene)");
				}
				else if (ImGui::MenuItem("关闭当前场景(Close Current Scene)"))
				{
					EngineEvent::getInstance()->dispatch({ EngineEventID::EE_PopScene });
				}

				ImGui::Separator();

				if (ImGui::MenuItem("加载模型(LoadModel)"))
				{
					auto path = MyGUIContext::openFile("fbx,pmx,obj");
					if (!path.empty())
					{
						if (!SceneManager::empty())
						{
							auto model = ResourceManager::loadAndSave<Model>(path.string());
							model->generate();
						}
					}
				}

				ImGui::Separator();

				if (ImGui::MenuItem("退出(Exit)"))
				{
					Engine::close();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("资源(Resource)"))
			{
				if (ImGui::MenuItem("创建相机(Create Camera)"))
				{
					MyGUIContext::createCamera();
				}

				if (ImGui::MenuItem("创建区域光(Create Sphere)"))
				{
					MyGUIContext::createDirectionLight();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("创建立方体(Create Cube)"))
				{
					MyGUIContext::createBuildinModel("Cube");
				}

				if (ImGui::MenuItem("创建球体(Create Sphere)"))
				{
					MyGUIContext::createBuildinModel("Sphere");
				}

				if (ImGui::MenuItem("创建方形(Create Square)"))
				{
					MyGUIContext::createBuildinModel("Square");
				}


				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("教程(Tutorial)"))
			{
				for (auto scene : SceneManager::getAllSceneArray())
				{
					if (ImGui::MenuItem(scene->getName().data()))
					{
						EngineEvent::getInstance()->dispatch({ EngineEventID::EE_PopScene });
						EngineEvent::getInstance()->dispatch({ EngineEventID::EE_PushScene, scene });
					}
				}

				ImGui::EndMenu();
			}

			//----------------------------------
			if (ImGui::BeginMenu("窗口(Windows)"))
			{
				if (ImGui::MenuItem("物体信息(ObjectInfo)"))
				{
					MyObjectInfoWindow::create(this->getHost())->setFocus();
				}

				if (ImGui::MenuItem("总览(Overview)"))
				{
					MyOverviewWindow::create(this->getHost())->setFocus();
				}

				if (ImGui::MenuItem("场景(Scene)"))
				{
					MySceneWindow::create(this->getHost())->setFocus();
				}

				if (ImGui::MenuItem("文件目录(FileSystem)"))
				{
					MyResourceWindow::create(this->getHost())->setFocus();
				}

				if (ImGui::MenuItem("光照管理器(LightingManager)"))
				{
					MyLightingWindow::create(this->getHost())->setFocus();
				}

				if (ImGui::MenuItem("信息(Log)"))
				{
					MyLogWindow::create(this->getHost())->setFocus();
				}

				if (ImGui::MenuItem("内存信息(GCInfo)"))
				{
					MyGCInfoWindow::create(this->getHost())->setFocus();
				}

				if (ImGui::MenuItem("着色器编辑器(ShaderEditor)"))
				{
					MyShaderEditorWindow::create(this->getHost())->setFocus();
				}

				if (ImGui::MenuItem("帧缓存查看器(FrameBufferViewer)"))
				{
					MyFrameBufferViewerWindow::create(this->getHost())->setFocus();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

	}
}
