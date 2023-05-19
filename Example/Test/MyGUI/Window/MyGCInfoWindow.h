#pragma once

#include "../MyGUIContext.h"

class MyGCInfoWindow : public GUIWindow
{

public:
	MyGCInfoWindow();
	virtual ~MyGCInfoWindow();

protected:
	void onRender() override;
	void onUpdate() override;

};


