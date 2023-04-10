#include "MyInputer.h"
#include "MyController.h"

MyInputer::MyInputer()
	: mLockMouse(true)
	, mLastX(0)
	, mLastY(0)
	, mController(nullptr)
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
		position += mController->getFront() * speed;
	}
	else if (system->getKeyDown(GLFW_KEY_S))
	{
		transform = true;
		position -= mController->getFront() * speed;
	}

	if (system->getKeyDown(GLFW_KEY_A))
	{
		transform = true;
		position -= mController->getRight() * speed;
	}
	else if (system->getKeyDown(GLFW_KEY_D))
	{
		transform = true;
		position += mController->getRight() * speed;
	}

	if (system->getKeyDown(GLFW_KEY_R))
	{
		transform = true;
		position += mController->getUp() * speed;
	}
	else if (system->getKeyDown(GLFW_KEY_F))
	{
		transform = true;
		position -= mController->getUp() * speed;

	}
// 	if (system->getKeyDown(GLFW_KEY_Q))
// 	{
// 		mController->roll(-speed);
// 	}
// 	else if (system->getKeyDown(GLFW_KEY_E))
// 	{
// 		mController->roll(speed);
// 	}

	float current_x;
	float current_y;
	system->getMousePosition(current_x, current_y);
	Statistic::mousePosition.x = current_x;
	Statistic::mousePosition.y = current_y;

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

			Statistic::mouseOffset.x = offset_x;
			Statistic::mouseOffset.y = offset_y;

			mLastX = current_x;
			mLastY = current_y;

			mController->yawPitch(offset_x, offset_y);
		}
	}
	else
	{
		mLockMouse = true;
	}

	if (transform)
	{
		mController->getTransform()->translate(position);
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

	mController->updateVector();
}

void MyInputer::onEnter()
{

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
