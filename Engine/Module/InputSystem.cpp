#include "InputSystem.h"
#include "Inputer.h"

#include <GLFW/glfw3.h>

namespace tezcat::Tiny::Module
{
	IMPLEMENT_DELEGATE_INSTANCE(InputSystem);

	InputSystem::InputSystem() :
		m_Window(nullptr)
	{
		REGISTER_DELEGATE_INSTANCE(InputSystem);
	}

	void InputSystem::push(Inputer* inputer)
	{
		if (!m_InputerStack.empty())
		{
			m_InputerStack.top()->onPause();
		}
		inputer->onEnter();
		m_InputerStack.push(inputer);
	}

	void InputSystem::pop()
	{
		m_InputerStack.top()->onExit();
		m_InputerStack.pop();
	}

	void InputSystem::mouseButtonCallBack(int button, int action, int mods)
	{
		m_InputerStack.top()->mouseButtonCallBack(button, action, mods);
	}

	void InputSystem::mouseCursorPosCallBack(double xpos, double ypos)
	{
		m_InputerStack.top()->mouseCursorPosCallBack(xpos, ypos);
	}

	void InputSystem::mouseScrollPosCallBack(double xoffset, double yoffset)
	{
		m_InputerStack.top()->mouseScrollPosCallBack(xoffset, yoffset);
	}

	void InputSystem::processInput()
	{
		m_InputerStack.top()->processInput(this);
	}

	bool InputSystem::getKeyDown(int key)
	{
		return glfwGetKey(m_Window, key) == GLFW_PRESS;
	}

	bool InputSystem::getKeyUp(int key)
	{
		return glfwGetKey(m_Window, key) == GLFW_RELEASE;
	}

	bool InputSystem::getMouseButtonDown(int key)
	{
		return glfwGetMouseButton(m_Window, key) == GLFW_PRESS;
	}

	bool InputSystem::getMouseButtonUp(int key)
	{
		return glfwGetMouseButton(m_Window, key) == GLFW_RELEASE;
	}

	void InputSystem::getMousePosition(float& x, float& y)
	{
		glfwGetCursorPos(m_Window, &m_MouseX, &m_MouseY);
		x = static_cast<float>(m_MouseX);
		y = static_cast<float>(m_MouseY);
	}
}

