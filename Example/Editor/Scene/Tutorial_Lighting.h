#pragma once

#include "TutorialHelper.h"


class Tutorial_Lighting : public Scene
{
	Tutorial_Lighting(const std::string& name);
	TINY_OBJECT_H(Tutorial_Lighting, Scene)

public:
	virtual ~Tutorial_Lighting();

	void onEnter() override;
	void onExit() override;

};
