#pragma once
#include "../MyGUIContext.h"

namespace tezcat::Editor
{
	class MyLightingWindow : public GUIWindow
	{
		TINY_EDITOR_WINDOW_INSTANCE_H(MyLightingWindow)
	public:

	protected:
		void onRender() override;

	private:
		float mSkyboxLod;

		Texture2D* mCurrentHDR;
		std::vector<Texture2D*> mHDR;
	};


}
