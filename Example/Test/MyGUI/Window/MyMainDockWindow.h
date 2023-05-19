#pragma once

#include "../MyGUIContext.h"

class MyMainDockWindow : public GUIWindow
{
public:
	MyMainDockWindow();
	virtual ~MyMainDockWindow();

protected:
	void onUpdate() override;
	void onRender() override;

public:
	bool begin() override;
	void end() override;

private:
	ImGuiWindowFlags mWindowFlags;
	ImGuiDockNodeFlags mDockspaceFlags;

	std::vector<Texture2D*> mHDR;
};



