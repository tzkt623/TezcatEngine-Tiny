#pragma once

#include "Tiny.h"

class MyInputer : public InputerT<MyInputer>
{
public:
	MyInputer();
	virtual ~MyInputer();

public:
	// Í¨¹ý Inputer ¼Ì³Ð
	void processInput(InputSystem* system) override;

	void onEnter() override;

	void onExit() override;

	void onPause() override;

	void mouseButtonCallBack(int button, int action, int mods) override;

	void mouseCursorPosCallBack(double xpos, double ypos) override;

	void mouseScrollPosCallBack(double xoffset, double yoffset) override;

private:
	bool m_LockMouse;
	float m_LastX;
	float m_LastY;

	Camera* m_Camera;
};

