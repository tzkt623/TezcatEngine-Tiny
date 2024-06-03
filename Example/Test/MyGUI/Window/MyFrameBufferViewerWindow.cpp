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

		static ImVec2 size(128, 128);
		static ImVec2 uv0(0, 1);
		static ImVec2 uv1(1, 0);
		auto& buffers = FrameBufferManager::getAllFrameBuffer();
		static auto flags = ImGuiTableFlags_BordersInner
			| ImGuiTableFlags_BordersOuter
			| ImGuiTableFlags_Resizable;
		if (ImGui::BeginTable("##FrameBufferViewerWindow", 2, flags))
		{		
			for (auto& pair : buffers)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text(pair.first.data());

				ImGui::TableNextColumn();
				auto frame = pair.second;
				auto& attachments = frame->getAttachmentes();
				for (uint32 i = 0; i < attachments.size(); i++)
				{
					ImGui::BeginGroup();
					auto tex = attachments[i];
					//auto [w, h, l] = tex->getSizeWHL();

					switch (tex->getTextureType())
					{
					case TextureType::TextureRender2D:
						break;
					case TextureType::Texture2D:
					{
						ImGui::Image((ImTextureID)tex->getTextureID(), size, uv0, uv1);
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
					if (i != attachments.size() - 1)
					{
						ImGui::SameLine();
					}
				}
				
			}
			

			ImGui::EndTable();
		}
	}
}
