#pragma once

#include "Tiny.h"

class MyScene : public Scene
{
public:
	MyScene(const std::string& name);

	void onEnter() override;


protected:
	void initGUI();

public:
};


