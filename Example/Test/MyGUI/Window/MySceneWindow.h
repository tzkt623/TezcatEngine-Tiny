#pragma once

#include "../MyGUIContext.h"

namespace tezcat::Editor
{
	class MySceneWindow : public GUIWindow
	{
		TINY_EDITOR_WINDOW_INSTANCE_H(MySceneWindow);

	protected:
		bool begin() override;
		void onRender() override;

		void drawInfo(const ImVec2& pos);

	private:
		Texture2D* mColorBuffer;
		std::vector<float> mFrameTimeBuffer;
		bool mIsPlaying;
		ImVec2 mViewPortPos;
		ImVec2 mMousePos;
		ImVec2 mFramePos;
	};
}
