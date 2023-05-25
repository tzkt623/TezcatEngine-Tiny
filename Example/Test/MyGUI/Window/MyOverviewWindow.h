#pragma once

#include "../MyGUIContext.h"

class MyOverviewWindow : public GUIWindow
{
	CreateInstanceH(MyOverviewWindow);

public:
	void init() override;
	void onRender() override;

private:
	GameObject* mSelectedGameObject;
};

