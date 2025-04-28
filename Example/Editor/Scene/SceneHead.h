#pragma once

#include "MyMainScene.h"
#include "Tutorial_Triangle.h"
#include "Tutorial_Texture2D.h"
#include "Tutorial_Cube.h"
#include "Tutorial_Camera.h"
#include "Tutorial_FrameBuffer.h"
#include "Tutorial_Lighting.h"
#include "Tutorial_Normal.h"

class SceneHelper
{
public:
	static void prepareTutorialScene()
	{
		Tutorial_Triangle::create("Tutorial Triangle")->prepare();
		Tutorial_Texture2D::create("Tutorial Texture2D")->prepare();
		Tutorial_Cube::create("Tutorial Cube")->prepare();
		Tutorial_Camera::create("Tutorial Camera")->prepare();
		Tutorial_Normal::create("Tutorial Normal")->prepare();
		Tutorial_FrameBuffer::create("Tutorial FrameBuffer")->prepare();
		Tutorial_Lighting::create("Tutorial Lighting")->prepare();
	}
};
