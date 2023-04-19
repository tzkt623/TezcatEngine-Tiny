#pragma once

#include "../MyGUIContext.h"

class MyOverviewWindow : public GUIWindow
{
public:
	MyOverviewWindow();
	virtual ~MyOverviewWindow();

	void onSelectObject(GameObject* object);
private:

protected:
	void onRender() override;

	void drawCamera(Component* com);
	void drawDirectionalLight(Component* com);
	void drawMeshRenderer(Component* com);

	void onUpdate() override;


private:
	bool mOpen;
	GameObject* mGameObject;
};


