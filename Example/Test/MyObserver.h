#pragma once

#include "Tiny.h"


class MyObserver : public IRenderObserver
{
public:
	MyObserver();
	virtual ~MyObserver();

public:
	void submit(BaseGraphics* graphics, Shader* shader) override;
};

