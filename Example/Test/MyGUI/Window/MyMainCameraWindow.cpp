#include "MyMainCameraWindow.h"



MyMainCameraWindow::MyMainCameraWindow()
	: GUIWindow(u8"主相机信息")
{

}

MyMainCameraWindow::~MyMainCameraWindow()
{
}

void MyMainCameraWindow::init()
{

}

void MyMainCameraWindow::onUpdate()
{
	auto camera = CameraMgr::getInstance()->getMainCamera();
	auto transform = camera->getTransform();

	ImGui::Text("操作<CTRL> [W A S D] [R:Up] [F:Down]");
	ImGui::InputFloat3("位置(Position)", glm::value_ptr(transform->getPosition()));
	ImGui::InputFloat3("旋转(Rotation)", glm::value_ptr(transform->getRotation()));
	ImGui::InputFloat3("缩放(Scale)", glm::value_ptr(transform->getScale()));
	ImGui::Spacing();

	auto up = transform->getUp();
	auto forward = transform->getForward();
	auto right = transform->getRight();
	ImGui::InputFloat3("上(Up)", glm::value_ptr(up));
	ImGui::InputFloat3("前(Forward)", glm::value_ptr(forward));
	ImGui::InputFloat3("右(Right)", glm::value_ptr(right));
	ImGui::Spacing();

	ImGui::Text("视图矩阵(ViewMatrix)");
	MyGUIContext::matrix4(camera->getViewMatrix());
	ImGui::Spacing();

	ImGui::Text("模型矩阵(ModelMatrix)");
	MyGUIContext::matrix4(camera->getTransform()->getModelMatrix());
}
