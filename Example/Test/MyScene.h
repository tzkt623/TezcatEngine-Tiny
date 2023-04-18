#pragma once

#include "Tiny.h"

class MyScene : public Scene
{
public:
	MyScene(const std::string& name);

	void onEnter() override;

	void createGates(float gateWidth, float gateHigh);

	void createPaintings();

	void createPlane();

	void createCubes0();

	void createTransparentObject();

	void createInfinitePlane();

	void createDirectionLight();

	void createPBR();


protected:
	void initGUI();

public:
};


