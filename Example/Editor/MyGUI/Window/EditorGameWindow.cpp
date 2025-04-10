/*
	Copyright (C) 2025 Tezcat(特兹卡特) tzkt623@qq.com

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

#include "EditorGameWindow.h"

namespace tezcat::Editor
{
	TINY_EDITOR_WINDOW_INSTANCE_CPP(EditorGameWindow);
	EditorGameWindow::EditorGameWindow()
		: GUIWindow("游戏(Game)")
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

	EditorGameWindow::~EditorGameWindow()
	{
		TINY_EDITOR_WINDOW_DELETE_INSTACNE();
	}

	bool EditorGameWindow::begin()
	{
		return ImGui::Begin(this->getName(), 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar);
	}

	void EditorGameWindow::onRender()
	{
		GUIWindow::onRender();

		EditorContext::IsFocusOnGameView = false;

		if (ImGui::BeginChild("##Game"))
		{
			if (!SceneManager::isEmpty() && CameraManager::isDataValied())
			{
				//mViewPortPos = ImGui::GetItemRectMin();
				if (mColorBuffer)
				{
					ImVec2 display_size, offset, uv0, uv1, texture_size;
					auto main_camera = CameraManager::getMainCamera().lock();
					if (main_camera)
					{
						texture_size = ImVec2((float)mColorBuffer->getWidth(), (float)mColorBuffer->getHeight());

						ImGuiHelper::fitImageToRect(texture_size, ImGui::GetWindowSize(), display_size, offset, uv0, uv1);
						main_camera->setViewRect(0, 0, display_size.x, display_size.y);
						//EditorContext::sViewPortSize = display_size;

						ImGui::SetCursorPos(offset);
						ImGui::Image((ImTextureID)mColorBuffer->getTextureID()
									, display_size
									, uv0
									, uv1);

						if (ImGui::IsItemHovered())
						{
							EditorContext::IsFocusOnGameView = true;
						}
					}
					else
					{
						texture_size = ImVec2((float)mColorBuffer->getWidth(), (float)mColorBuffer->getHeight());
						ImGuiHelper::fitImageToRect(texture_size, ImGui::GetWindowSize(), display_size, offset, uv0, uv1);

						ImGui::SetCursorPos(offset);
						ImGui::Image((ImTextureID)mColorBuffer->getTextureID()
									, display_size
									, ImVec2(0, 1)
									, ImVec2(1, 0));
					}
				}
				else
				{
					const char* str = "No Camera";
					auto size = ImGui::CalcTextSize(str);
					auto avail = ImGui::GetContentRegionAvail();
					ImVec2 pos((avail.x - size.x) * 0.5f, (avail.y - size.y) * 0.5f);
					ImGui::SetCursorPos(pos);
					ImGui::Text(str);

					//texture_size = ImVec2((float)mColorBuffer->getWidth(), (float)mColorBuffer->getHeight());
					//ImGuiHelper::fitImageToRect(texture_size, ImGui::GetWindowSize(), display_size, offset, uv0, uv1);
					//
					//ImGui::SetCursorPos(offset);
					//ImGui::Image((ImTextureID)mColorBuffer->getTextureID()
					//			, display_size
					//			, ImVec2(0, 1)
					//			, ImVec2(1, 0));
				}

				//mFramePos.x = mViewPortPos.x + offset.x;
				//mFramePos.y = mViewPortPos.y + offset.y;
				//
				//auto mouse_pos = ImGui::GetMousePos();
				//mMousePos.x = (mouse_pos.x - mFramePos.x);
				//mMousePos.y = ImGui::GetItemRectSize().y - (mouse_pos.y - mFramePos.y);
				//
				//if (ImGui::IsItemHovered())
				//{
				//	if (!ImGuizmo::IsOver() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				//	{
				//		if (GameObjectManager::allowPickObject())
				//		{
				//			int32_t pos[2]
				//			{
				//				mMousePos.x,
				//				mMousePos.y
				//			};
				//			EngineEvent::getInstance()->dispatch({ EngineEventID::EE_ReadObjectID, pos });
				//		}
				//	}
				//}
			}
		}

		ImGui::EndChild();
	}
}
