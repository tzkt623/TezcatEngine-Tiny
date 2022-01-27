#include "MyGUI.h"

MyInfoWindow::MyInfoWindow() :
	GUIWindow(u8"统计信息"),
	m_LabelPass(new GUIText(u8"PassCount")),
	m_LabelFPS(new GUIText(u8"FPS"))
{

}

MyInfoWindow::~MyInfoWindow()
{

}

void MyInfoWindow::init()
{
	this->addChild(m_LabelFPS);
	this->addChild(m_LabelPass);
}

void MyInfoWindow::onUpdate()
{
	m_LabelFPS->setData(Tools::stringFormat(
		"FPS: %.1f(%.3f ms/Frame)",
		ImGui::GetIO().Framerate,
		1000.0f / ImGui::GetIO().Framerate));

	m_LabelPass->setData(Tools::stringFormat("PassCount: %d", Statistic::PassCount));
}



//------------------------------------------------------
MyObjectWindow::MyObjectWindow():
	GUIWindow(u8"物体信息"),
	m_Position(new GUIDragFloat3(u8"坐标"))
{

}

MyObjectWindow::~MyObjectWindow()
{

}

void MyObjectWindow::init()
{
	m_Position->postFunction = [this](float* data)
	{
		auto camera = CameraManager::getInstance()->getCamera("Main");
		camera->getTransform()->setPosition(data);
	};
	this->addChild(m_Position);
}


void MyObjectWindow::onUpdate()
{
	auto camera = CameraManager::getInstance()->getCamera("Main");
	auto position = camera->getTransform()->getPosition();
	m_Position->setFloat3(position.x, position.y, position.z);
}
