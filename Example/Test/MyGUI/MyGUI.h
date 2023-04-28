#pragma once

#include "Window/MyViewPortWindow.h"
#include "Window/MyOverviewWindow.h"
#include "Window/MyInfoWindow.h"
#include "Window/MyLogWindow.h"
#include "Window/MyMainCameraWindow.h"
#include "Window/MyObjectWindow.h"
#include "Window/MyMainDockWindow.h"
#include "Window/MyGCInfoWindow.h"



class MyPBRWindow : public GUIWindow
{
public:
	MyPBRWindow();
	~MyPBRWindow();

	void init() override;

protected:
	void onUpdate() override;
	void onRender() override;
};


