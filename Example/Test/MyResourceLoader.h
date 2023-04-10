#pragma once

#include "Tiny.h"

class MyResourceLoader : public ResourceLoader
{
public:
	MyResourceLoader();

	void prepareEngine(Engine* engine) override;
	void prepareResource(Engine* engine) override;
	void prepareGame(Engine* engine) override;
	void initYourShaderParam() override;
};
