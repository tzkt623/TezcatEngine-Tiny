#pragma once

#include "../MyGUIContext.h"

class TransformWidget : public GUIWidget
{
public:
	TransformWidget();


protected:
	void onRender() override;
	void onUpdate() override;


private:
	glm::vec3 mPosition;
	glm::vec3 mRotation;
	glm::vec3 mScale;
};


