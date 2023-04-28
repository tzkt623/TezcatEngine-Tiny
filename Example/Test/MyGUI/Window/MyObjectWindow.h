#pragma once

#include "../MyGUIContext.h"

class MyObjectWindow : public GUIWindow
{
public:
	MyObjectWindow();
	~MyObjectWindow();

	void init() override;
protected:
	void onUpdate() override;

private:
	GameObject* mSelectedGameObject;
	GUIDragFloat3* mPosition;
};

