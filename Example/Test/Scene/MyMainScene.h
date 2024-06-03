#pragma once

#include "Tiny.h"

class MyMainScene : public Scene
{
	MyMainScene(const std::string& name);
	TINY_OBJECT_H(MyMainScene, Scene)

public:
	void createGates(float gateWidth, float gateHigh);
	void createPaintings();
	void createPlane();
	void createCubes0();
	void createTransparentObject();
	void createInfinitePlane();
	void createDirectionLight();
	void createPBR();

protected:
	virtual void onEnter() override;
	virtual void onExit() override;

private:
	GameObject* mController;
};

