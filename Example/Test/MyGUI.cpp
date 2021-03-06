#include "MyGUI.h"

MyInfoWindow::MyInfoWindow()
	: GUIWindow(u8"统计信息")
	, m_LabelGPU(new GUIText(u8"GPU"))
	, m_LabelMemory(new GUIText(u8"Memory"))
	, m_LabelLogicTime(new GUIText(u8"LogicTime"))
	, m_LabelRenderTime(new GUIText(u8"RenderTime"))
	, m_LabelPass(new GUIText(u8"PassCount"))
	, m_LabelFPS(new GUIText(u8"FPS"))
	, m_LabelDrawCall(new GUIText(u8"DrawCall"))
	, m_MousePosition(new GUIDragFloat2(u8"鼠标位置"))
	, m_MouseOffset(new GUIDragFloat2(u8"鼠标偏移"))
{

}

MyInfoWindow::~MyInfoWindow()
{
	delete m_LabelGPU;
	delete m_LabelPass;
	delete m_LabelFPS;
	delete m_MousePosition;
	delete m_MouseOffset;
	delete m_LabelMemory;
	delete m_LabelRenderTime;
	delete m_LabelLogicTime;
}

void MyInfoWindow::init()
{
	m_LabelGPU->setData(Tools::stringFormat("GPU: %s", Statistic::GPU));
	this->addChild(m_LabelGPU);
	this->addChild(m_LabelMemory);
	this->addChild(m_LabelLogicTime);
	this->addChild(m_LabelRenderTime);
	this->addChild(m_LabelFPS);
	this->addChild(m_LabelPass);
	this->addChild(m_LabelDrawCall);
	this->addChild(m_MousePosition);
	this->addChild(m_MouseOffset);
}

void MyInfoWindow::onUpdate()
{
	m_LabelMemory->setData(Tools::stringFormat("Memory: %.3f kb", Statistic::getMemoryUse() / 1024.0f));
	m_LabelLogicTime->setData(Tools::stringFormat("LogicTime: %.1f ms", Statistic::LogicTime));
	m_LabelRenderTime->setData(Tools::stringFormat("RenderTime: %.1f ms", Statistic::RenderTime));

	m_LabelFPS->setData(Tools::stringFormat("FPS: %.1f(%.3f ms/Frame)",
		GUIFunction::getFrameRate(), GUIFunction::getFrameRateTime()));

	m_LabelPass->setData(Tools::stringFormat("PassCount: %d", Statistic::PassCount));
	m_LabelDrawCall->setData(Tools::stringFormat("DrawCount: %d", Statistic::DrawCall));

	m_MousePosition->setFloat2(glm::value_ptr(Statistic::mousePosition));
	m_MouseOffset->setFloat2(glm::value_ptr(Statistic::mouseOffset));
}



//------------------------------------------------------
MyObjectWindow::MyObjectWindow() :
	GUIWindow(u8"物体信息"),
	m_Position(new GUIDragFloat3(u8"坐标"))
{

}

MyObjectWindow::~MyObjectWindow()
{

}

void MyObjectWindow::init()
{

}


void MyObjectWindow::onUpdate()
{

}

//------------------------------------
MyMainCameraWindow::MyMainCameraWindow() :
	GUIWindow(u8"主相机信息"),
	m_Info(new GUIText(u8"操作")),
	m_Position(new GUIDragFloat3(u8"坐标")),
	m_Rotation(new GUIDragFloat3(u8"旋转")),
	m_Right(new GUIDragFloat3(u8"右方")),
	m_Up(new GUIDragFloat3(u8"上方")),
	m_Front(new GUIDragFloat3(u8"前方")),
	m_MainCamera(nullptr)
{

}

MyMainCameraWindow::~MyMainCameraWindow()
{
}

void MyMainCameraWindow::init()
{
	m_MainCamera = SG<CameraManager>::getInstance()->getMainCamera();

	this->addChild(m_Info);
	this->addChild(m_Position);
	this->addChild(m_Rotation);
	this->addChild(m_Right);
	this->addChild(m_Up);
	this->addChild(m_Front);

	m_Position->postFunction = [this](float* data)
	{
		m_MainCamera->getTransform()->setPosition(data);
	};


	// 	m_Up->postFunction = [this](float* data)
	// 	{
	// 		m_MainCamera->setPosition(data);
	// 	};
}

void MyMainCameraWindow::onUpdate()
{
	m_Info->setData(Tools::fromU8(u8"控制<CTRL>\n[W A S D]\n[R:Up] [F:Down]"));

	auto position = m_MainCamera->getTransform()->getPosition();
	m_Position->setFloat3(glm::value_ptr(position));

	auto rotation = m_MainCamera->getTransform()->getRotation();
	m_Rotation->setFloat3(glm::value_ptr(rotation));

	auto up = m_MainCamera->getUp();
	m_Up->setFloat3(glm::value_ptr(up));

	auto front = m_MainCamera->getFront();
	m_Front->setFloat3(glm::value_ptr(front));

	auto right = m_MainCamera->getRight();
	m_Right->setFloat3(glm::value_ptr(right));
}
