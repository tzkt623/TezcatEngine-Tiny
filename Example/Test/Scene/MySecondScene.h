#pragma once

#include "Tiny.h"

class MySeconedScene : public Scene
{
	MySeconedScene(const std::string& name);
	TINY_RTTI_H(MySeconedScene);
	TINY_Factory(MySeconedScene);

public:
	virtual ~MySeconedScene();


	void onEnter() override;
	void onExit() override;
	void onPause() override;
	void onResume() override;
};
