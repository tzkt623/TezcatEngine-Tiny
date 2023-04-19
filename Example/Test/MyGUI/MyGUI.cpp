#include "MyGUI.h"


MyPBRWindow::MyPBRWindow()
	: GUIWindow(u8"PBR Material")
{

}

MyPBRWindow::~MyPBRWindow()
{

}

void MyPBRWindow::init()
{

}

void MyPBRWindow::onUpdate()
{

}

void MyPBRWindow::onRender()
{
	const float speed = 0.005f;
	const float min = 0.0f;
	const float max = 1.0f;

	if (Statistic::MatPBR_Albedo)
	{
		ImGui::DragFloat3("Albedo", glm::value_ptr(Statistic::MatPBR_Albedo->value), speed, min, max);
	}

	if (Statistic::MatPBR_Metallic)
	{
		ImGui::DragFloat("Metallic", &Statistic::MatPBR_Metallic->value, speed, min, max);
	}

	if (Statistic::MatPBR_Roughness)
	{
		ImGui::DragFloat("Roughness", &Statistic::MatPBR_Roughness->value, speed, min, max);
	}

	if (Statistic::MatPBR_AO)
	{
		ImGui::DragFloat("AO", &Statistic::MatPBR_AO->value, speed, min, max);
	}
}



