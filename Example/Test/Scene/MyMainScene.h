#pragma once

#include "Tiny.h"

class MyMainScene : public Scene
{
	MyMainScene(const std::string& name);
	TINY_RTTI_H(MyMainScene);
	TINY_Factory(MyMainScene);

public:
	void createGates(float gateWidth, float gateHigh);
	void createPaintings();
	void createPlane();
	void createCubes0();
	void createTransparentObject();
	void createInfinitePlane();
	void createDirectionLight();
	void createPBR();
	void createSkybox();

protected:
	void onEnter() override;



private:
	GameObject* mController;
};

