#pragma once

#include "Tiny.h"

class MyResourceLoader : public ResourceLoader
{
public:
	MyResourceLoader();

	void prepareEngine(Engine* engine) override;
	void prepareScene(Engine* engine) override;
	void prepareResource(Engine* engine) override;
	void initYourShaderParam() override;

private:
	void createSomeMode();
};