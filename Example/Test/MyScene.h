#pragma once

#include "Tiny.h"

class MyScene : public Scene
{
public:
	MyScene(const std::string& name);

	void onEnter() override;


protected:
	void initGUI();

public:
	static TextureRenderBuffer2D* rbWorld1;
	static TextureRenderBuffer2D* rbWorld2;

	static FrameBuffer* fbWorld1;
	static FrameBuffer* fbWorld2;
};


