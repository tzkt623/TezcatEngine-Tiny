#include "MyTextureViewerWindow.h"

MyTextureViewerWindow::MyTextureViewerWindow(const std::string& name)
	: GUIWindow(name)
	, mTex(nullptr)
{

}

MyTextureViewerWindow::~MyTextureViewerWindow()
{

}

void MyTextureViewerWindow::setTexture(Texture* tex)
{
	mTex = tex;
}

bool MyTextureViewerWindow::begin()
{
	return ImGui::Begin(mName.c_str(), &mIsOpen, ImGuiWindowFlags_NoScrollbar);
}

void MyTextureViewerWindow::onRender()
{
	if (ImGui::BeginChild("Viewport"))
	{
		switch (mTex->getTextureType())
		{
		case TextureType::Texture2D:
			ImGui::Image((ImTextureID)mTex->getTextureID()
				, ImGui::GetWindowSize()
				, ImVec2(0, 1)
				, ImVec2(1, 0));
			break;
		case TextureType::TextureCube:
			break;
		default:
			break;
		}


	}
	ImGui::EndChild();
}

void MyTextureViewerWindow::onUpdate()
{

}

