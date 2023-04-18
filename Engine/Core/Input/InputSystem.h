#pragma once

#include "../Head/CppHead.h"
#include "Platform/OpenGL/GLHead.h"
#include "Utility/Singleton.h"

struct GLFWwindow;

using namespace tezcat::Tiny::Utility;
namespace tezcat::Tiny::Core
{
	class Inputer;
	class TINY_API InputSystem
	{
	public:
		InputSystem();

		void push(Inputer* inputer);
		void pop();

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

