#pragma once

#include "Tiny.h"

class MyResourceLoader : public ResourceLoader
{
	void prepareEngine(Core::Engine* engine) override;
	void prepareScene(Engine* engine) override;
};