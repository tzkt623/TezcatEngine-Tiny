#pragma once
#include "../MyGUIContext.h"

class MyLightingWindow : public GUIWindow
{
	CreateInstanceH(MyLightingWindow);
public:

protected:
	void onRender() override;

private:
	float mSkyboxLod;

	Texture2D* mCurrentHDR;
	std::vector<Texture2D*> mHDR;
};


