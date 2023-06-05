#pragma once

#include "../MyGUIContext.h"

class MyTextureViewerWindow : public GUIWindow
{
public:
	MyTextureViewerWindow(const std::string& name);
	virtual ~MyTextureViewerWindow();

	void setTexture(Texture* tex);
	void loadTexture(const std::filesystem::path& path);

	bool begin() override;
protected:

	void onRender() override;
private:
	Texture* mTex;
	ImVec2 mSize;
};

