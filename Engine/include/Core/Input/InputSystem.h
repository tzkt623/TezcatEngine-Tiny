#pragma once

#include "../Head/CppHead.h"
#include "Platform/OpenGL/GLHead.h"
#include "../Tool/Tool.h"


struct GLFWwindow;

namespace tezcat::Tiny
{
	class Inputer;
	class TINY_API InputSystem
	{
	public:
		InputSystem();

		void push(Inputer* inputer);
		void pop();
		Inputer* currentInputer() { return mInputerStack.top(); }

	public:
		void setWindow(GLFWwindow* val) { mWindow = val; }

		void mouseButtonCallBack(int button, int action, int mods);
		void mouseCursorPosCallBack(double xpos, double ypos);
		void mouseScrollPosCallBack(double xoffset, double yoffset);

		void update();

	public:
		bool getKeyDown(int key);
		bool getKeyUp(int key);

		bool getMouseButtonDown(int key);
		bool getMouseButtonUp(int key);

		void getMousePosition(float& x, float& y);

	private:
		GLFWwindow* mWindow;

		double mMouseX;
		double mMouseY;
		std::stack<Inputer*> mInputerStack;
	};

	using InputSys = SG<InputSystem>;
}

