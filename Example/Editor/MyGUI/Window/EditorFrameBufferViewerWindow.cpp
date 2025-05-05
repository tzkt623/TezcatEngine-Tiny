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

#include "EditorFrameBufferViewerWindow.h"

namespace tezcat::Editor
{
	TINY_EDITOR_WINDOW_INSTANCE_CPP(EditorFrameBufferViewerWindow);

	EditorFrameBufferViewerWindow::EditorFrameBufferViewerWindow()
		: GUIWindow("帧缓存查看器(FrameBufferViewer)")
		, mSelectBuffer(nullptr)
	{
		EngineEvent::getInstance()->addListener(EngineEventID::EE_PopScene, this
			, [this](const EventData& data)
			{
				mSelectBuffer = nullptr;
			});
	}

	EditorFrameBufferViewerWindow::~EditorFrameBufferViewerWindow()
	{
		TINY_EDITOR_WINDOW_DELETE_INSTACNE();
	}

	void EditorFrameBufferViewerWindow::init()
	{

	}

	void EditorFrameBufferViewerWindow::onRender()
	{
		GUIWindow::onRender();

		static ImVec2 size(512, 512);
		static ImVec2 uv0(0, 1);
		static ImVec2 uv1(1, 0);
		static ImVec2 offset;

		ImGui::BeginChild("##FrameBufferViewerWindow", ImVec2(240.0f, ImGui::GetContentRegionAvail().y), true);

		auto& frame_buffers = FrameBufferManager::getAllFrameBuffer();
		if (!frame_buffers.empty())
		{
			ImGuiListClipper clipper;
			clipper.Begin(frame_buffers.size(), ImGui::GetTextLineHeightWithSpacing());
			while (clipper.Step())
			{
				for (auto& pair : frame_buffers)
				{
					ImGui::Selectable(pair.first.data(), mSelectBuffer == pair.second);
					if (ImGui::IsItemHovered())
					{
						if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
						{
							mSelectBuffer = pair.second;
						}
					}
				}
			}
			clipper.End();
		}

		ImGui::EndChild();


		ImGui::SameLine();

		ImGui::BeginChild("##FrameBufferTextureWindow", ImVec2(0.0f, 0.0f), false, ImGuiWindowFlags_HorizontalScrollbar);

		if (mSelectBuffer)
		{
			auto& attachments = mSelectBuffer->getAttachmentes();
			for (uint32 i = 0; i < attachments.size(); i++)
			{
				ImGui::BeginGroup();
				auto tex = attachments[i];
				auto [w, h, l] = tex->getSizeWHL();

				switch (tex->getTextureType())
				{
				case TextureType::TextureRender2D:
					break;
				case TextureType::Texture2D:
				{
					ImVec2 display_size, uv00, uv01, tex_size(w, h);
					ImGuiHelper::fitImageToRect(size, tex_size, display_size, offset);
					ImGui::Image((ImTextureID)tex->getTextureID(), display_size, uv0, uv1);
					//ImGui::SetCursorPos(offset);
					break;
				}
				//不能直接显示cube,否则会爆炸
				case TextureType::TextureCube:
					//ImGui::Image((ImTextureID)tex->getTextureID(), size, uv0, uv1);
					break;
				default:
					//ImGui::Image(nullptr, size);
					break;
				}
				ImGui::Text(tex->getName().c_str());
				ImGui::EndGroup();
				//if (i != attachments.size() - 1)
				//{
				//	ImGui::SameLine();
				//}
			}
		}

		ImGui::EndChild();

	}
}
