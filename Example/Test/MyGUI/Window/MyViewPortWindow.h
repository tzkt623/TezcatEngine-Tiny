#pragma once

#include "../MyGUIContext.h"

namespace tezcat::Editor
{
	class MyViewPortWindow : public GUIWindow
	{
		TINY_EDITOR_WINDOW_INSTANCE_H(MyViewPortWindow);

	protected:
		bool begin() override;
		void onRender() override;

		void calculate(const ImVec2& inTextureSize, const ImVec2& inWindowSize, ImVec2& outDisplaySize, ImVec2& outOffset, ImVec2& outUV0, ImVec2& outUV1);
		void drawInfo(const ImVec2& pos);

	private:
		Texture2D* mColorBuffer;
		std::vector<float> mFrameTimeBuffer;
		bool mIsPlaying;
		ImVec2 mViewPortPos;
		ImVec2 mMousePos;
		ImVec2 mFramePos;

	public:
	};



}
