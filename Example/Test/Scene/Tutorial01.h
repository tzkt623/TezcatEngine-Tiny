#pragma once

#include "include/Tiny.h"


class Tutorial01 : public Scene
{
	Tutorial01(const std::string& name);
	TINY_OBJECT_H(Tutorial01, Scene)

public:
	virtual ~Tutorial01();


	void onEnter() override;
	void onExit() override;
	void onPause() override;
	void onResume() override;

private:
	Vertex* mVertex = nullptr;
	ReplacedPipelinePass* mPass = nullptr;
	RenderObserver* mObserver = nullptr;
};


