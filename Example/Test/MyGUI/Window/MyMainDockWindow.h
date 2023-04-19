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
	void beginParent() override;
	void endParent() override;

private:
	ImGuiWindowFlags mWindowFlags;
	ImGuiDockNodeFlags mDockspaceFlags;
};



