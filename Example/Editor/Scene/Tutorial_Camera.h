#pragma once


#include "TutorialHelper.h"


class Tutorial_Camera : public Scene
{
	Tutorial_Camera(const std::string& name);
	TINY_OBJECT_H(Tutorial_Camera, Scene)

public:
	virtual ~Tutorial_Camera();


	void onEnter() override;
	void onExit() override;
	void onPause() override;
	void onResume() override;

private:
	Vertex* mVertex = nullptr;
	ReplacedPipelinePass* mPass = nullptr;
	RenderObserver* mObserver = nullptr;

	float4x4 mProjectionMatrix;
	float4x4 mViewMatrix;
};

