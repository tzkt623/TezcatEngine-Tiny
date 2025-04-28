#pragma once

#include "TutorialHelper.h"


class Tutorial_Normal : public Scene
{
	Tutorial_Normal(const std::string& name);
	TINY_OBJECT_H(Tutorial_Normal, Scene)

public:
	virtual ~Tutorial_Normal();

	void onEnter() override;
	void onExit() override;

};
