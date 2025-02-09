#pragma once

#include "include/Tiny.h"

class MyInputer : public InputerT<MyInputer>
{
public:
	MyInputer();
	virtual ~MyInputer();

public:
	void processInput(InputSystem* system) override;

	void onEnter() override;
	void onExit() override;
	void onPause() override;

	void mouseButtonCallBack(int button, int action, int mods) override;
	void mouseCursorPosCallBack(double xpos, double ypos) override;
	void mouseScrollPosCallBack(double xoffset, double yoffset) override;

	void setController(FlyController* val)
	{
		mController = TinyWeakRef<FlyController>(val);
	}

private:
	bool mLockMouse;
	float mLastX;
	float mLastY;

	TinyWeakRef<FlyController> mController;
};

