#pragma once
#include "../EditorContext.h"

namespace tezcat::Editor
{
	class EditorLightingWindow : public GUIWindow
	{
		TINY_EDITOR_WINDOW_INSTANCE_H(EditorLightingWindow)
	public:

	protected:
		void onRender() override;

	private:
		float mSkyboxLod;

		Texture2D* mCurrentHDR;
		std::vector<Texture2D*> mHDR;
	};


}
