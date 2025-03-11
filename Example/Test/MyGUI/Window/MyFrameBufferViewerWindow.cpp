#include "MyFrameBufferViewerWindow.h"

namespace tezcat::Editor
{
	TINY_EDITOR_WINDOW_INSTANCE_CPP(MyFrameBufferViewerWindow)

	MyFrameBufferViewerWindow::MyFrameBufferViewerWindow()
		: GUIWindow("帧缓存查看器(FrameBufferViewer)")
	{

	}

	MyFrameBufferViewerWindow::~MyFrameBufferViewerWindow()
	{
		TINY_EDITOR_WINDOW_DELETE_INSTACNE();
	}

	void MyFrameBufferViewerWindow::init()
	{

	}

	void MyFrameBufferViewerWindow::onRender()
	{
		GUIWindow::onRender();

		static ImVec2 size(512, 512);
		static ImVec2 uv0(0, 1);
		static ImVec2 uv1(1, 0);
		static ImVec2 offset;

		ImGui::BeginChild("##FrameBufferViewerWindow", ImVec2(240, ImGui::GetContentRegionAvail().y), true);

		auto& frame_buffers = FrameBufferManager::getAllFrameBuffer();

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
					MyTextureSizeHelper::fitImageToRect(size, tex_size, display_size, offset);
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
