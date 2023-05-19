#pragma once

#include "../MyGUIContext.h"

class MyTextureViewerWindow : public GUIWindow
{
public:
	MyTextureViewerWindow(const std::string& name);
	virtual ~MyTextureViewerWindow();

	void setTexture(Texture* tex);

	bool begin() override;

private:
	Texture* mTex;

protected:

	void onRender() override;
	void onUpdate() override;

};

