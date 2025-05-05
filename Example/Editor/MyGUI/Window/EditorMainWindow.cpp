/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "EditorMainWindow.h"
#include "EditorTextureViewerWindow.h"
#include "EditorResourceWindow.h"
#include "EditorShaderEditorWindow.h"
#include "EditorObjectInfoWindow.h"
#include "EditorOverviewWindow.h"
#include "EditorGCInfoWindow.h"
#include "EditorLogWindow.h"
#include "EditorSceneWindow.h"
#include "EditorLightingWindow.h"
#include "EditorFrameBufferViewerWindow.h"
#include "EditorPipelineWindow.h"
#include "EditorGameWindow.h"

#include "../../EditorEvent.h"
namespace tezcat::Editor
{
	EditorMainWindow::EditorMainWindow()
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
		EditorEvent::get()->addListener(EditorEventID::Window_OpenShaderEditor, this, [this](const EventData& data)
		{
			auto path = (std::filesystem::path*)data.userData;
			EditorShaderEditorWindow::create(*path, this->getHost())->setFocus();
		});

		EditorEvent::get()->addListener(EditorEventID::Window_OpenImageViwer, this, [this](const EventData& data)
		{
			auto path = (std::filesystem::path*)data.userData;
			auto window = new EditorTextureViewerWindow(path->filename().string());
			window->loadTexture(*path);
			window->open(this->getHost());
		});

		EngineEvent::getInstance()->addListener(EngineEventID::EE_BeforeSceneExit, this,
		[](const EventData& data)
		{
			EditorContext::clearOnPopScene();
		});
	}

	EditorMainWindow::~EditorMainWindow()
	{

	}

	void EditorMainWindow::init()
	{
		EditorSceneWindow::create(this->getHost());
		EditorGameWindow::create(this->getHost());

		EditorObjectInfoWindow::create(this->getHost());
		EditorOverviewWindow::create(this->getHost());
		EditorResourceWindow::create(this->getHost());
		EditorLogWindow::create(this->getHost());
		EditorLightingWindow::create(this->getHost());
		EditorPipelineWindow::create(this->getHost());
		//MyGCInfoWindow::create(this->getHost());
	}

	bool EditorMainWindow::begin()
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, 5.0f);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		return ImGui::Begin("##MainWindow", 0, mWindowFlags);
	}

	void EditorMainWindow::end()
	{
		ImGui::End();
	}
	void EditorMainWindow::onRender()
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
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("文件(File)"))
			{
				if (ImGui::MenuItem("新场景(NewScene)"))
				{
					SceneManager::switchScene(Scene::create("NewScene"));
				}

				if (SceneManager::isEmpty())
				{
					ImGui::TextColored(ImVec4(0.2f, 0.2f, 0.2f, 1.0f), "关闭当前场景(Close Current Scene)");
				}
				else if (ImGui::MenuItem("关闭当前场景(Close Current Scene)"))
				{
					EditorContext::closeScene();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("加载模型(LoadModel)"))
				{
					auto path = EditorContext::openFile("fbx,pmx,obj");
					if (!path.empty())
					{
						if (!SceneManager::isEmpty())
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
					EditorContext::createCamera();
				}

				if (ImGui::MenuItem("创建区域光(Create Sphere)"))
				{
					EditorContext::createDirectionLight();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("创建立方体(Create Cube)"))
				{
					EditorContext::createBuildinModel("Cube");
				}

				if (ImGui::MenuItem("创建球体(Create Sphere)"))
				{
					EditorContext::createBuildinModel("Sphere");
				}

				if (ImGui::MenuItem("创建方形(Create Square)"))
				{
					EditorContext::createBuildinModel("Square");
				}


				ImGui::EndMenu();
			}

			//----------------------------------
			if (ImGui::BeginMenu("窗口(Windows)"))
			{
				if (ImGui::MenuItem("场景(Scene)"))
				{
					EditorSceneWindow::create(this->getHost())->setFocus();
				}

				if (ImGui::MenuItem("游戏(Game)"))
				{
					EditorGameWindow::create(this->getHost())->setFocus();
				}

				if (ImGui::MenuItem("物体信息(ObjectInfo)"))
				{
					EditorObjectInfoWindow::create(this->getHost())->setFocus();
				}

				if (ImGui::MenuItem("总览(Overview)"))
				{
					EditorOverviewWindow::create(this->getHost())->setFocus();
				}

				if (ImGui::MenuItem("文件目录(FileSystem)"))
				{
					EditorResourceWindow::create(this->getHost())->setFocus();
				}

				if (ImGui::MenuItem("光照管理器(LightingManager)"))
				{
					EditorLightingWindow::create(this->getHost())->setFocus();
				}

				if (ImGui::MenuItem("信息(Log)"))
				{
					EditorLogWindow::create(this->getHost())->setFocus();
				}

				if (ImGui::MenuItem("内存信息(GCInfo)"))
				{
					EditorGCInfoWindow::create(this->getHost())->setFocus();
				}

				if (ImGui::MenuItem("着色器编辑器(ShaderEditor)"))
				{
					EditorShaderEditorWindow::create(this->getHost())->setFocus();
				}

				if (ImGui::MenuItem("帧缓存查看器(FrameBufferViewer)"))
				{
					EditorFrameBufferViewerWindow::create(this->getHost())->setFocus();
				}

				if (ImGui::MenuItem("管线查看器(PipelineViewer)"))
				{
					EditorPipelineWindow::create(this->getHost())->setFocus();
				}

				ImGui::EndMenu();
			}


			if (ImGui::BeginMenu("教程(Tutorial)"))
			{
				for (auto scene : SceneManager::getAllSceneArray())
				{
					if (ImGui::MenuItem(scene->getName().data()))
					{
						EditorContext::changeScene(scene);
					}
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if (ImGui::BeginMenuBar())
		{
			ImGui::Spacing();
			if (ImGui::MenuItem("T", 0, EditorContext::SelectedObjectOperation == ImGuizmo::OPERATION::TRANSLATE))
			{
				EditorContext::SelectedObjectOperation = ImGuizmo::OPERATION::TRANSLATE;
			}

			if (ImGui::MenuItem("R", 0, EditorContext::SelectedObjectOperation == ImGuizmo::OPERATION::ROTATE))
			{
				EditorContext::SelectedObjectOperation = ImGuizmo::OPERATION::ROTATE;
			}

			if (ImGui::MenuItem("S", 0, EditorContext::SelectedObjectOperation == ImGuizmo::OPERATION::SCALE))
			{
				EditorContext::SelectedObjectOperation = ImGuizmo::OPERATION::SCALE;
			}

			ImGui::Spacing();
			ImGui::Spacing();
			switch (EditorContext::SelectedObjectMode)
			{
			case ImGuizmo::LOCAL:
				if (ImGui::Button("Local"))
				{
					EditorContext::SelectedObjectMode = ImGuizmo::MODE::WORLD;
				}
				break;
			case ImGuizmo::WORLD:
				if (ImGui::Button("World"))
				{
					EditorContext::SelectedObjectMode = ImGuizmo::MODE::LOCAL;
				}
				break;
			default:
				break;
			}

			float width = ImGui::GetItemRectSize().x;
			float pos_x = ImGui::GetCursorPosX();

			ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * 0.5f);

			const char* str_play = "Playing";
			auto size = ImGui::CalcTextSize(str_play);
			if (ImGui::Selectable(str_play, EditorContext::IsPlaying, 0, size))
			{
				EditorContext::IsPlaying = !EditorContext::IsPlaying;
				if (!EditorContext::IsPlaying)
				{
					EditorContext::IsPause = false;
					//发送恢复消息

				}
			}

			const char* str_pause = "Pause";
			size = ImGui::CalcTextSize(str_pause);
			if (ImGui::Selectable(str_pause, EditorContext::IsPause, 0, size))
			{
				EditorContext::IsPause = !EditorContext::IsPause;
			}

			ImGui::EndMenuBar();
		}
	}
}
