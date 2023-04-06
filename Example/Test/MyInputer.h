#pragma once

#include "Tiny.h"

class MyController;
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

	void setController(MyController* val) { mController = val; }

private:
	bool m_LockMouse;
	float m_LastX;
	float m_LastY;

	Camera* mCamera;
	MyController* mController;
};

