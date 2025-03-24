#pragma once

#include "TutorialHelper.h"

class Tutorial_FrameBuffer : public Scene
{
	Tutorial_FrameBuffer(const std::string& name);
	TINY_OBJECT_H(Tutorial_FrameBuffer, Scene)

public:
	virtual ~Tutorial_FrameBuffer();

	void onEnter() override;
	void onExit() override;
	void onPause() override;
	void onResume() override;

private:
	float3 randomColor();

private:
	Vertex* mVertex = nullptr;
	ReplacedPipelinePass* mPass = nullptr;
	RenderObserver* mObserver = nullptr;
	FrameBuffer* mFrameBuffer = nullptr;

	std::default_random_engine mRandom;
	std::uniform_real_distribution<float> mGenerator;
	float3 mColor;
	decltype(std::chrono::high_resolution_clock::now()) mTimeBegin;
};

