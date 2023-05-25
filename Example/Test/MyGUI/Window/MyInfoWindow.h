#pragma once

#include "../MyGUIContext.h"

class MyInfoWindow : public GUIWindow
{
public:
	MyInfoWindow();
	~MyInfoWindow();

	void init() override;

protected:
	void onRender() override;

private:
	std::vector<float> mFrameTimeBuffer;
};


