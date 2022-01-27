#include "MyInputer.h"

MyInputer::MyInputer() :
	m_Camera(nullptr),
	m_LockMouse(true)
{

}

MyInputer::~MyInputer()
{
}

void MyInputer::processInput(InputSystem* system)
{
	bool transform = false;
	glm::vec3 position(0.0f);

	if (system->getKeyDown(GLFW_KEY_W))
	{
		transform = true;
		position.y += Engine::getDeltaTime();
	}
	else if (system->getKeyDown(GLFW_KEY_S))
	{
		transform = true;
		position.y -= Engine::getDeltaTime();
	}

	if (system->getKeyDown(GLFW_KEY_A))
	{
		transform = true;
		position.x -= Engine::getDeltaTime();
	}
	else if (system->getKeyDown(GLFW_KEY_D))
	{
		transform = true;
		position.x += Engine::getDeltaTime();
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
