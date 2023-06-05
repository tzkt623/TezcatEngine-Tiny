#pragma once

#include "Tiny.h"

class MyEngineIniter : public EngineIniter
{
public:
	MyEngineIniter();

	void prepareEngine(Engine* engine) override;
	void prepareResource(Engine* engine) override;
	void prepareGame(Engine* engine) override;
	void initYourShaderParam() override;
};
