#include "TransformWidget.h"

TransformWidget::TransformWidget()
	: GUIWidget("Transform")
{

}

void TransformWidget::onRender()
{
	ImGui::DragFloat3("Position", &mPosition.x);
	ImGui::DragFloat3("Rotation", &mRotation.x);
	ImGui::DragFloat3("Scale", &mScale.x);
}

void TransformWidget::onUpdate()
{

}

