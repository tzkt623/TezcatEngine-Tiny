#pragma once

#include "../MyGUIContext.h"

class MyViewPortWindow : public GUIWindow
{
public:
	MyViewPortWindow();
	virtual ~MyViewPortWindow();

protected:
	void onUpdate() override;
	void calculate(const ImVec2& inTextureSize, const ImVec2& inWindowSize, ImVec2& outDisplaySize, ImVec2& outOffset, ImVec2& outUV0, ImVec2& outUV1);
	void onRender() override;



private:
	TextureBuffer2D* mColorBuffer;
	TextureBuffer2D* mDepthBuffer;
};
