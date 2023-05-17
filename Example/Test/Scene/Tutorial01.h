#pragma once

#include "Tiny.h"


class MyObserver;
class Tutorial01 : public Scene
{
	Tutorial01(const std::string& name);
	TINY_RTTI_H(Tutorial01);
	TINY_Factory(Tutorial01);

public:
	virtual ~Tutorial01();


	void onEnter() override;
	void onExit() override;
	void onPause() override;
	void onResume() override;

private:
	Vertex* mVertex = nullptr;
	MyObserver* mObserver = nullptr;
};


