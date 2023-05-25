#pragma once

#include "../MyGUIContext.h"

class MyMainWindow : public GUIWindow
{
public:
	MyMainWindow();
	virtual ~MyMainWindow();

	void init() override;

protected:
	void onRender() override;

public:
	bool begin() override;
	void end() override;

private:
	ImGuiWindowFlags mWindowFlags;
	ImGuiDockNodeFlags mDockspaceFlags;
};



