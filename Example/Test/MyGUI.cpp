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
MyMainCameraWindow::MyMainCameraWindow()
	: GUIWindow(u8"主相机信息")
	, m_Info(new GUIText(u8"信息"))
	, mPosition(new GUIDragFloat3(u8"坐标(Position)"))
	, m_Rotation(new GUIDragFloat3(u8"旋转(Rotation)"))
	, m_Right(new GUIDragFloat3(u8"右方(Right)"))
	, m_Up(new GUIDragFloat3(u8"上方(Up)"))
	, m_Front(new GUIDragFloat3(u8"前方(Front)"))
	, mMainCamera(nullptr)
	, mViewMatrix(new GUIMatrix4(u8"视图矩阵"))
	, mModelMatrix(new GUIMatrix4(u8"模型矩阵"))
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
	this->addChild(mViewMatrix);
	this->addChild(mModelMatrix);

	mPosition->postFunction = [this](float* data)
	{
		mMainCamera->getTransform()->setPosition(data[0], data[1], data[2]);
	};


	mViewMatrix->setMat(&mMainCamera->getViewMatrix());
	mModelMatrix->setMat(&mMainCamera->getTransform()->getModelMatrix());

	// 	m_Up->postFunction = [this](float* data)
	// 	{
	// 		m_MainCamera->setPosition(data);
	// 	};
}

void MyMainCameraWindow::onUpdate()
{
	mMainCamera = SG<CameraManager>::getInstance()->getMainCamera();
	auto transform = mMainCamera->getTransform();
	m_Info->setData(StringTool::fromU8(u8"操作<CTRL>\n[W A S D]\n[R:Up] [F:Down]"));

	auto position = transform->getPosition();
	mPosition->setFloat3(glm::value_ptr(position));

	auto rotation = transform->getRotation();
	m_Rotation->setFloat3(glm::value_ptr(rotation));

	auto up = transform->getUp();
	m_Up->setFloat3(glm::value_ptr(up));

	auto front = transform->getForward();
	m_Front->setFloat3(glm::value_ptr(front));

	auto right = transform->getRight();
	m_Right->setFloat3(glm::value_ptr(right));
}

//----------------------------------------------

MyPBRWindow::MyPBRWindow()
	: GUIWindow(u8"PBR Material")
	, mAlbedo(new GUIDragFloat3(u8"Albedo"))
	, mMetallic(new GUIDragFloat1(u8"Metallic"))
	, mRoughness(new GUIDragFloat1(u8"Roughness"))
	, mAO(new GUIDragFloat1(u8"AO"))
{

}

MyPBRWindow::~MyPBRWindow()
{

}

void MyPBRWindow::init()
{
	this->addChild(mAlbedo);
	this->addChild(mMetallic);
	this->addChild(mRoughness);
	this->addChild(mAO);

	mAlbedo->setMinMax(0.0f, 1.0f);
	mAlbedo->setSpeed(0.005f);
	mAlbedo->postFunction = [](float* data)
	{
		if (Statistic::MatPBR_Albedo)
		{
			Statistic::MatPBR_Albedo->value = glm::vec3(data[0], data[1], data[2]);
		}
	};

	mMetallic->setMinMax(0.0f, 1.0f);
	mMetallic->setSpeed(0.005f);
	mMetallic->postFunction = [](float* data)
	{
		if (Statistic::MatPBR_Metallic)
		{
			Statistic::MatPBR_Metallic->value = *data;
		}
	};

	mRoughness->setMinMax(0.0f, 1.0f);
	mRoughness->setSpeed(0.005f);
	mRoughness->postFunction = [](float* data)
	{
		if (Statistic::MatPBR_Roughness)
		{
			Statistic::MatPBR_Roughness->value = *data;
		}
	};

	mAO->setMinMax(0.0f, 1.0f);
	mAO->setSpeed(0.005f);
	mAO->postFunction = [](float* data)
	{
		if (Statistic::MatPBR_AO)
		{
			Statistic::MatPBR_AO->value = *data;
		}
	};
}

void MyPBRWindow::onUpdate()
{

}
