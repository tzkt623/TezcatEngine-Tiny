#pragma once

#include "TutorialHelper.h"


class Tutorial_Texture2D : public Scene
{
	Tutorial_Texture2D(const std::string& name);
	TINY_OBJECT_H(Tutorial_Texture2D, Scene)

public:
	virtual ~Tutorial_Texture2D();


	void onEnter() override;
	void onExit() override;
	void onPause() override;
	void onResume() override;

private:
	Vertex* mVertex = nullptr;
	ReplacedPipelinePass* mPass = nullptr;
	RenderObserver* mObserver = nullptr;
};
