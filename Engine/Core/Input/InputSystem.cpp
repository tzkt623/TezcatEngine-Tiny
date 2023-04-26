#include "InputSystem.h"
#include "Inputer.h"

#include "../Tool/Tool.h"

namespace tezcat::Tiny
{
	InputSystem::InputSystem()
		: mWindow(nullptr)
		, mMouseX(0)
		, mMouseY(0)
	{
		SG<InputSystem>::attach(this);

		this->push(EmptyInputer::getInstance());
	}

	void InputSystem::push(Inputer* inputer)
	{
		if (!mInputerStack.empty())
		{
			mInputerStack.top()->onPause();
		}
		inputer->onEnter();
		mInputerStack.push(inputer);
	}

	void InputSystem::pop()
	{
		mInputerStack.top()->onExit();
		mInputerStack.pop();
	}

	void InputSystem::mouseButtonCallBack(int button, int action, int mods)
	{
		mInputerStack.top()->mouseButtonCallBack(button, action, mods);
	}

	void InputSystem::mouseCursorPosCallBack(double xpos, double ypos)
	{
		mInputerStack.top()->mouseCursorPosCallBack(xpos, ypos);
	}

	void InputSystem::mouseScrollPosCallBack(double xoffset, double yoffset)
	{
		mInputerStack.top()->mouseScrollPosCallBack(xoffset, yoffset);
	}

	void InputSystem::update()
	{
		mInputerStack.top()->processInput(this);
	}

	bool InputSystem::getKeyDown(int key)
	{
		return glfwGetKey(mWindow, key) == GLFW_PRESS;
	}

	bool InputSystem::getKeyUp(int key)
	{
		return glfwGetKey(mWindow, key) == GLFW_RELEASE;
	}

	bool InputSystem::getMouseButtonDown(int key)
	{
		return glfwGetMouseButton(mWindow, key) == GLFW_PRESS;
	}

	bool InputSystem::getMouseButtonUp(int key)
	{
		return glfwGetMouseButton(mWindow, key) == GLFW_RELEASE;
	}

	void InputSystem::getMousePosition(float& x, float& y)
	{
		glfwGetCursorPos(mWindow, &mMouseX, &mMouseY);
		x = static_cast<float>(mMouseX);
		y = static_cast<float>(mMouseY);
	}
}

