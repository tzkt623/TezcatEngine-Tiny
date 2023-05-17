#pragma once

#include "Tiny.h"


class MyObserver : public IRenderObserver
{
public:
	MyObserver();
	virtual ~MyObserver();

public:
	void submit(Shader* shader) override;
};

