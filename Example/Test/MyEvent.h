#pragma once
#include "include/Tiny.h"

class MyEvent
{
	MyEvent() = delete;
	~MyEvent() = delete;

public:
	static Event* get() { return mEvent; }

private:
	static Event* mEvent;
};

enum MyEventID : EventID
{
	Window_ObjectSelected = 0,
	Window_OpenShaderEditor,
	Window_OpenImageViwer,
	Count
};
