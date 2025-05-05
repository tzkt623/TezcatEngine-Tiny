/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

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

#include "EditorInputer.h"
#include "MyController.h"

namespace tezcat::Editor
{
	EditorInputer::EditorInputer()
		: mLockMouse(true)
		, mLastX(0)
		, mLastY(0)
	{

	}

	EditorInputer::~EditorInputer()
	{
	}

	void EditorInputer::processInput(InputSystem* system)
	{
		auto ptr = mController.lock();
		if (!ptr)
		{
			return;
		}

		float speed = 100 * EngineContext::DeltaTime;
		bool flag = false;
		auto transform = mController->getTransform();
		glm::vec3 position(0.0f);

		if (system->getKeyDown(GLFW_KEY_W))
		{
			flag = true;
			position += mController->getForward() * speed;
		}
		else if (system->getKeyDown(GLFW_KEY_S))
		{
			flag = true;
			position -= mController->getForward() * speed;
		}

		if (system->getKeyDown(GLFW_KEY_A))
		{
			flag = true;
			position -= mController->getRight() * speed;
		}
		else if (system->getKeyDown(GLFW_KEY_D))
		{
			flag = true;
			position += mController->getRight() * speed;
		}

		if (system->getKeyDown(GLFW_KEY_R))
		{
			flag = true;
			position += mController->getUp() * speed;
		}
		else if (system->getKeyDown(GLFW_KEY_F))
		{
			flag = true;
			position -= mController->getUp() * speed;

		}
		if (system->getKeyDown(GLFW_KEY_Q))
		{
			mController->roll(-speed);
		}
		else if (system->getKeyDown(GLFW_KEY_E))
		{
			mController->roll(speed);
		}

		float rollspeed = 20 * EngineContext::DeltaTime;
		float current_x;
		float current_y;
		system->getMousePosition(current_x, current_y);
		Profiler::mousePosition.x = current_x;
		Profiler::mousePosition.y = current_y;

		if (system->getMouseButtonDown(GLFW_MOUSE_BUTTON_2))
		{
			if (mLockMouse)
			{
				mLockMouse = false;
				mLastX = current_x;
				mLastY = current_y;
			}
			else
			{
				float offset_x = current_x - mLastX;
				float offset_y = mLastY - current_y;

				Profiler::mouseOffset.x = offset_x;
				Profiler::mouseOffset.y = offset_y;

				mLastX = current_x;
				mLastY = current_y;

				mController->yawPitch(-offset_x * rollspeed, offset_y * rollspeed);

				//mController->getTransform()->rotate(glm::vec3(offset_y * rollspeed, offset_x * rollspeed, 0));
			}
		}
		else
		{
			mLockMouse = true;
		}

		if (flag)
		{
			mController->translate(position);
		}
		else
		{
			// 		float radius = 5.0f;
			// 		float camX = glm::sin(glfwGetTime()) * radius;
			// 		float camZ = glm::cos(glfwGetTime()) * radius;
			// 		position.x = camX;
			// 		position.z = camZ;
			// 		m_Camera->getTransform()->setPosition(position);
		}

		//	mController->updateVector();
	}

	void EditorInputer::onEnter()
	{

	}

	void EditorInputer::onExit()
	{

	}

	void EditorInputer::onPause()
	{

	}

	void EditorInputer::mouseButtonCallBack(int button, int action, int mods)
	{

	}

	void EditorInputer::mouseCursorPosCallBack(double xpos, double ypos)
	{

	}

	void EditorInputer::mouseScrollPosCallBack(double xOffset, double yOffset)
	{
		float xpos = static_cast<float>(xOffset);
		float ypos = static_cast<float>(yOffset);

		if (mLockMouse)
		{
			mLastX = xpos;
			mLastY = ypos;
			mLockMouse = false;
			return;
		}

		float xoffset = xpos - mLastX;
		float yoffset = mLastY - ypos; // reversed since y-coordinates go from bottom to top

		mLastX = xpos;
		mLastY = ypos;

		//	m_Camera->ProcessMouseMovement(xoffset, yoffset);
	}
}
