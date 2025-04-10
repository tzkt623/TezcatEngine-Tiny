#pragma once

#include "TutorialHelper.h"


class Tutorial_Cube : public Scene
{
	Tutorial_Cube(const std::string& name);
	TINY_OBJECT_H(Tutorial_Cube, Scene)

public:
	virtual ~Tutorial_Cube();


	void onEnter() override;
	void onExit() override;
	void onPause() override;
	void onResume() override;

private:
	Vertex* mVertex = nullptr;
	ReplacedPipelinePass* mPass = nullptr;
	RenderObserver* mObserver = nullptr;
};
