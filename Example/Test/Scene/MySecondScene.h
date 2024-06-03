#pragma once

#include "Tiny.h"

class MySeconedScene : public Scene
{
	MySeconedScene(const std::string& name);
	TINY_OBJECT_H(MySeconedScene, Scene)

public:
	virtual ~MySeconedScene();


	void onEnter() override;
	void onExit() override;
	void onPause() override;
	void onResume() override;
};
