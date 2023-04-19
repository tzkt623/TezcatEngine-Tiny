#pragma once

#include "../MyGUIContext.h"

class MyMainCameraWindow : public GUIWindow
{
public:
	MyMainCameraWindow();
	~MyMainCameraWindow();

	void init() override;

protected:
	void onUpdate() override;
};



