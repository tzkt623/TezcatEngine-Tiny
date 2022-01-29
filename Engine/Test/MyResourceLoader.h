#pragma once

#include "Tiny.h"

class MyResourceLoader : public ResourceLoader
{
	void prepareEngine(Engine* engine) override;
	void prepareScene(Engine* engine) override;
	void prepareResource(Engine* engine) override;
};