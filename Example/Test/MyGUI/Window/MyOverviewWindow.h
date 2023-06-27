#pragma once

#include "../MyGUIContext.h"

class MyOverviewWindow : public GUIWindow
{
	CreateInstanceH(MyOverviewWindow);

public:
	void init() override;
	void buildTree(std::list<TinyWeakRef<Transform>>& children);
	void onRender() override;
	void end() override;

private:
	GameObject* mSelectedGameObject;
	Transform* mDragedTransform;
};

