#pragma once
#include <stack>
#include "Tools.h"
struct GLFWwindow;

namespace tezcat::Tiny::Module
{
	class Inputer;
	class InputSystem
	{
		CREATE_DELEGATE_INSTANCE(InputSystem)
	public:
		InputSystem();

		void push(Inputer* inputer);
		void pop();

	public:
		void setWindow(GLFWwindow* val) { m_Window = val; }

		void mouseButtonCallBack(int button, int action, int mods);

		void mouseCursorPosCallBack(double xpos, double ypos);

		void mouseScrollPosCallBack(double xoffset, double yoffset);

		void processInput();

	public:
		bool getKeyDown(int key);
		bool getKeyUp(int key);

		bool getMouseButtonDown(int key);
		bool getMouseButtonUp(int key);

		void getMousePosition(float& x, float& y);

	private:
		GLFWwindow* m_Window;

		double m_MouseX;
		double m_MouseY;
		std::stack<Inputer*> m_InputerStack;
	};
}

