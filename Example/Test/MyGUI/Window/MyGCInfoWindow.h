#pragma once

#include "../MyGUIContext.h"

class MyGCInfoWindow : public GUIWindow
{

public:
	MyGCInfoWindow();
	virtual ~MyGCInfoWindow();

	void begin() override;
	void end() override;

protected:
	void onRender() override;
	void onUpdate() override;

};


