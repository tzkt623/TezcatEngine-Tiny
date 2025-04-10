#pragma once

#include "TutorialHelper.h"


class Tutorial_Triangle : public Scene
{
	Tutorial_Triangle(const std::string& name);
	TINY_OBJECT_H(Tutorial_Triangle, Scene)

public:
	virtual ~Tutorial_Triangle();


	void onEnter() override;
	void onExit() override;
	void onPause() override;
	void onResume() override;

private:
	Vertex* mVertex = nullptr;
	ReplacedPipelinePass* mPass = nullptr;
	RenderObserver* mObserver = nullptr;
};
