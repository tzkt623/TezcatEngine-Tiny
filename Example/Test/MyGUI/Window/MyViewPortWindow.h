#pragma once

#include "../MyGUIContext.h"

class MyViewPortWindow : public GUIWindow
{
	CreateInstanceH(MyViewPortWindow);

protected:
	bool begin() override;
	void onRender() override;

	void calculate(const ImVec2& inTextureSize, const ImVec2& inWindowSize, ImVec2& outDisplaySize, ImVec2& outOffset, ImVec2& outUV0, ImVec2& outUV1);
	void drawInfo(const ImVec2& pos);

private:
	Texture2D* mColorBuffer;
	std::vector<float> mFrameTimeBuffer;
};



