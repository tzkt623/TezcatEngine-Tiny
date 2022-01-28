#include "MyInputer.h"
#include <iostream>

MyInputer::MyInputer() :
	m_Camera(nullptr),
	m_LockMouse(true),
	m_LastX(0),
	m_LastY(0)
{

}

MyInputer::~MyInputer()
{
}

void MyInputer::processInput(InputSystem* system)
{
	float speed = 10 * Engine::getDeltaTime();
	bool transform = false;
	glm::vec3 position(0.0f);

	if (system->getKeyDown(GLFW_KEY_W))
	{
		transform = true;
		position += m_Camera->getFront() * speed;
	}
	else if (system->getKeyDown(GLFW_KEY_S))
	{
		transform = true;
		position -= m_Camera->getFront() * speed;
	}

	if (system->getKeyDown(GLFW_KEY_A))
	{
		transform = true;
		position -= m_Camera->getRight() * speed;
	}
	else if (system->getKeyDown(GLFW_KEY_D))
	{
		transform = true;
		position += m_Camera->getRight() * speed;
	}

	float current_x;
	float current_y;
	system->getMousePosition(current_x, current_y);
	Statistic::mousePosition.x = current_x;
	Statistic::mousePosition.y = current_y;

	if (system->getMouseButtonDown(GLFW_MOUSE_BUTTON_2))
	{
		if (m_LockMouse)
		{
			m_LockMouse = false;
			m_LastX = current_x;
			m_LastY = current_y;
		}
		else
		{
			float offset_x = current_x - m_LastX;
			float offset_y = m_LastY - current_y;

			Statistic::mouseOffset.x = offset_x;
			Statistic::mouseOffset.y = offset_y;

			m_LastX = current_x;
			m_LastY = current_y;

			m_Camera->rotateCamera(offset_x, offset_y);
		}
	}
	else
	{
		m_LockMouse = true;
	}

	if (transform)
	{
		m_Camera->getTransform()->translate(position);
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
}

void MyInputer::onEnter()
{
	m_Camera = Engine::getInstance()->getCameraManager()->getCamera("Main");
}

void MyInputer::onExit()
{

}

void MyInputer::onPause()
{

}

void MyInputer::mouseButtonCallBack(int button, int action, int mods)
{

}

void MyInputer::mouseCursorPosCallBack(double xpos, double ypos)
{

}

void MyInputer::mouseScrollPosCallBack(double xOffset, double yOffset)
{
	float xpos = static_cast<float>(xOffset);
	float ypos = static_cast<float>(yOffset);

	if (m_LockMouse)
	{
		m_LastX = xpos;
		m_LastY = ypos;
		m_LockMouse = false;
		return;
	}

	float xoffset = xpos - m_LastX;
	float yoffset = m_LastY - ypos; // reversed since y-coordinates go from bottom to top

	m_LastX = xpos;
	m_LastY = ypos;

	//	m_Camera->ProcessMouseMovement(xoffset, yoffset);
}
