#include "MySceneWindow.h"

namespace tezcat::Editor
{
	TINY_EDITOR_WINDOW_INSTANCE_CPP(MySceneWindow);
	MySceneWindow::MySceneWindow()
		: GUIWindow("场景(Scene)")
		, mColorBuffer(nullptr)
		, mIsPlaying(true)
	{
		EngineEvent::getInstance()->addListener(EngineEventID::EE_AfterSceneEnter, this,
			[this](const EventData& data)
			{
				mColorBuffer = (Texture2D*)FrameBufferManager::getMainFrameBufferBuildin()->getAttachmentes().at(0);
			});

		EngineEvent::getInstance()->addListener(EngineEventID::EE_AfterSceneExit, this,
			[this](const EventData& data)
			{
				mColorBuffer->clearInGPU();
				mColorBuffer = nullptr;
			});
	}

	MySceneWindow::~MySceneWindow()
	{
		TINY_EDITOR_WINDOW_DELETE_INSTACNE();
	}

	bool MySceneWindow::begin()
	{
		return ImGui::Begin(this->getName(), 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar);
	}

	void MySceneWindow::onRender()
	{
		GUIWindow::onRender();

		//if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
		//{
		//	InputSys::getInstance()->update();
		//}

		if (ImGui::BeginMenuBar())
		{
			static bool bInfo;
			ImGui::MenuItem("Info", 0, &bInfo);
			if (bInfo)
			{
				auto pos = ImGui::GetItemRectMin();
				pos.y += ImGui::GetItemRectSize().y;

				this->drawInfo(pos);
			}

			ImGui::EndMenuBar();
		}

		/*
		if (mIsPlaying)
		{
			if (ImGui::Button("Pause"))
			{
				mIsPlaying = false;
			}
		}
		else
		{
			if (ImGui::Button("Play"))
			{
				mIsPlaying = true;
			}
		}
		*/

		if (ImGui::BeginChild("##Scene"))
		{
			if (SceneManager::isSceneRunning())
			{
				mViewPortPos = ImGui::GetItemRectMin();

				if (mColorBuffer)
				{
					ImVec2 display_size, offset, uv0, uv1, texture_size;
					auto camera_data = CameraManager::getData();
					if (camera_data.lock() && camera_data->getMainCamera())
					{
						texture_size = ImVec2((float)mColorBuffer->getWidth(), (float)mColorBuffer->getHeight());

						ImGuiHelper::fitImageToRect(texture_size, ImGui::GetWindowSize(), display_size, offset, uv0, uv1);
						//this->calculate(texture_size, ImGui::GetWindowSize(), display_size, offset, uv0, uv1);

						camera_data->getMainCamera()->setViewRect(0, 0, display_size.x, display_size.y);
						MyGUIContext::sViewPortSize = display_size;

						ImGui::SetCursorPos(offset);
						ImGui::Image((ImTextureID)mColorBuffer->getTextureID()
									, display_size
									, uv0
									, uv1);
					}
					else
					{
						texture_size = ImVec2((float)mColorBuffer->getWidth(), (float)mColorBuffer->getHeight());
						//this->calculate(texture_size, ImGui::GetWindowSize(), display_size, offset, uv0, uv1);
						ImGuiHelper::fitImageToRect(texture_size, ImGui::GetWindowSize(), display_size, offset, uv0, uv1);

						ImGui::SetCursorPos(offset);
						ImGui::Image((ImTextureID)mColorBuffer->getTextureID()
									, display_size
									, ImVec2(0, 1)
									, ImVec2(1, 0));
					}

					mFramePos.x = mViewPortPos.x + offset.x;
					mFramePos.y = mViewPortPos.y + offset.y;

					auto mouse_pos = ImGui::GetMousePos();
					mMousePos.x = (mouse_pos.x - mFramePos.x);
					mMousePos.y = ImGui::GetItemRectSize().y - (mouse_pos.y - mFramePos.y);

					if (ImGui::IsItemHovered())
					{
						if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
						{
							if (GameObjectManager::allowPickObject())
							{
								int32_t pos[2]
								{
									mMousePos.x,
									mMousePos.y
								};
								EngineEvent::getInstance()->dispatch({ EngineEventID::EE_ReadObjectID, pos });
							}
						}

						InputSys::getInstance()->update();
					}
				}
			}

			ImGui::EndChild();
		}
	}

	void MySceneWindow::drawInfo(const ImVec2& pos)
	{
		ImGui::SetNextWindowPos(pos);
		ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		if (ImGui::Begin("状态(State)", 0, window_flags))
			//if (ImGui::BeginChild("状态(State)"))
		{
			//gpu
			ImGui::Text("GPU: %s", Profiler::GPU);

			//logic time
			ImGui::Text("LogicTime: %.1f ms", Profiler::LogicTime);

			//render time
			mFrameTimeBuffer.push_back((float)Profiler::RenderTime);
			if (mFrameTimeBuffer.size() > 40)
			{
				mFrameTimeBuffer.erase(mFrameTimeBuffer.begin());
			}
			ImGui::SetNextItemWidth(200);
			ImGui::PlotLines("Frame Times", &mFrameTimeBuffer[0], (int)mFrameTimeBuffer.size());
			ImGui::SameLine();
			ImGui::Text("%.1f ms", Profiler::RenderTime);

			//fps
			ImGui::Text("FPS: %.1f(%.3f ms/Frame)", GUIFunction::getFrameRate(), GUIFunction::getFrameRateTime());

			//draw
			ImGui::Text("PassCount: %d", Profiler::PassCount);
			ImGui::Text("DrawCount: %d", Profiler::DrawCall);
			ImGui::Separator();

			ImGui::Text("Memory");
			ImGui::Text("Used: %.3f mb", Profiler::getMemoryUse() / 1024.0f / 1024.0f);
			ImGui::Text("TotalID: %d", TinyGC::totalID());
			ImGui::SameLine();
			ImGui::Text("FreeID: %d", TinyGC::freeID());
			ImGui::SameLine();
			ImGui::Text("UsedID: %d", TinyGC::usedID());
			ImGui::Separator();

			ImGui::Text("ObjectCount: %d", GameObjectManager::getObjectCount());

			ImGui::Separator();

			//auto mouse_pos = ImGui::GetMousePos();
			//ImVec2 mouse_local(mouse_pos.x - mFramePos.x, mouse_pos.y - mFramePos.y);
			//mouse
			ImGui::InputFloat2("MousePosition", &mMousePos[0], "%.3f", ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat2("MouseOffset", glm::value_ptr(Profiler::mouseOffset), "%.3f", ImGuiInputTextFlags_ReadOnly);

			ImGui::End();
		}
	}
}
