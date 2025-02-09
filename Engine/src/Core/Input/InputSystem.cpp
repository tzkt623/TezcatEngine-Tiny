/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Core/Input/InputSystem.h"
#include "Core/Input/Inputer.h"

#include "Core/Tool/Tool.h"

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

