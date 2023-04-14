#include "MyGUI.h"

MyInfoWindow::MyInfoWindow()
	: GUIWindow(u8"信息(Info)")
	, m_LabelGPU(new GUIText(u8"GPU"))
	, m_LabelMemory(new GUIText(u8"Memory"))
	, m_LabelLogicTime(new GUIText(u8"LogicTime"))
	, m_LabelRenderTime(new GUIText(u8"RenderTime"))
	, m_LabelPass(new GUIText(u8"PassCount"))
	, m_LabelFPS(new GUIText(u8"FPS"))
	, m_LabelDrawCall(new GUIText(u8"DrawCall"))
	, m_MousePosition(new GUIDragFloat2(u8"MousePosition"))
	, m_MouseOffset(new GUIDragFloat2(u8"MouseOffset"))
	, mImageDepth(new GUIImage(u8"DepthImage"))
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
	delete mImageDepth;
}

void MyInfoWindow::init()
{
	m_LabelGPU->setData(StringTool::stringFormat("GPU: %s", Statistic::GPU));
	this->addChild(m_LabelGPU);
	this->addChild(m_LabelMemory);
	this->addChild(m_LabelLogicTime);
	this->addChild(m_LabelRenderTime);
	this->addChild(m_LabelFPS);
	this->addChild(m_LabelPass);
	this->addChild(m_LabelDrawCall);
	this->addChild(m_MousePosition);
	this->addChild(m_MouseOffset);
	this->addChild(mImageDepth);
}

void MyInfoWindow::onUpdate()
{
	m_LabelMemory->setData(StringTool::stringFormat("Memory: %.3f kb", Statistic::getMemoryUse() / 1024.0f));
	m_LabelLogicTime->setData(StringTool::stringFormat("LogicTime: %.1f ms", Statistic::LogicTime));
	m_LabelRenderTime->setData(StringTool::stringFormat("RenderTime: %.1f ms", Statistic::RenderTime));

	m_LabelFPS->setData(StringTool::stringFormat("FPS: %.1f(%.3f ms/Frame)",
		GUIFunction::getFrameRate(), GUIFunction::getFrameRateTime()));

	m_LabelPass->setData(StringTool::stringFormat("PassCount: %d", Statistic::PassCount));
	m_LabelDrawCall->setData(StringTool::stringFormat("DrawCount: %d", Statistic::DrawCall));

	m_MousePosition->setFloat2(glm::value_ptr(Statistic::mousePosition));
	m_MouseOffset->setFloat2(glm::value_ptr(Statistic::mouseOffset));
	
	mImageDepth->refresh();
}



//------------------------------------------------------
MyObjectWindow::MyObjectWindow() :
	GUIWindow(u8"物体信息(Object Info)"),
	mPosition(new GUIDragFloat3(u8"坐标"))
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
// 	auto& list = SceneMgr::getInstance()->getCurrentScene()->getObjectList();
// 	for (auto tran : list)
// 	{
// 		ImGui::Text(tran->getName().c_str());
// 	}
}

//------------------------------------
MyMainCameraWindow::MyMainCameraWindow() :
	GUIWindow(u8"主相机信息"),
	m_Info(new GUIText(u8"信息")),
	mPosition(new GUIDragFloat3(u8"坐标(Position)")),
	m_Rotation(new GUIDragFloat3(u8"旋转(Rotation)")),
	m_Right(new GUIDragFloat3(u8"右方(Right)")),
	m_Up(new GUIDragFloat3(u8"上方(Up)")),
	m_Front(new GUIDragFloat3(u8"前方(Front)")),
	mMainCamera(nullptr)
{

}

MyMainCameraWindow::~MyMainCameraWindow()
{
}

void MyMainCameraWindow::init()
{
	mMainCamera = SG<CameraManager>::getInstance()->getMainCamera();

	this->addChild(m_Info);
	this->addChild(mPosition);
	this->addChild(m_Rotation);
	this->addChild(m_Right);
	this->addChild(m_Up);
	this->addChild(m_Front);

	mPosition->postFunction = [this](float* data)
	{
		mMainCamera->getTransform()->setPosition(data);
	};


	// 	m_Up->postFunction = [this](float* data)
	// 	{
	// 		m_MainCamera->setPosition(data);
	// 	};
}

void MyMainCameraWindow::onUpdate()
{
	mMainCamera = SG<CameraManager>::getInstance()->getMainCamera();
	m_Info->setData(StringTool::fromU8(u8"操作<CTRL>\n[W A S D]\n[R:Up] [F:Down]"));

	auto position = mMainCamera->getTransform()->getPosition();
	mPosition->setFloat3(glm::value_ptr(position));

	auto rotation = mMainCamera->getTransform()->getRotation();
	m_Rotation->setFloat3(glm::value_ptr(rotation));

	auto up = mMainCamera->getUp();
	m_Up->setFloat3(glm::value_ptr(up));

	auto front = mMainCamera->getFront();
	m_Front->setFloat3(glm::value_ptr(front));

	auto right = mMainCamera->getRight();
	m_Right->setFloat3(glm::value_ptr(right));
}
