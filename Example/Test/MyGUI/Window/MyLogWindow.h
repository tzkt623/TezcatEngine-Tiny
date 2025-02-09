#pragma once

#include "../MyGUIContext.h"
#include "include/Tiny.h"
namespace tezcat::Editor
{
	class MyLogWindow : public GUIWindow
	{
		TINY_EDITOR_WINDOW_INSTANCE_H(MyLogWindow)

	protected:
		void onRender() override;

		void clear();
		void addLog(const char* fmt, ...) IM_FMTARGS(2);
		void addLog(const std::string& info);

	private:
		ImGuiTextBuffer mTextBuffer;
		ImGuiTextFilter mFilter;
		ImVector<int> mLineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
		bool mAutoScroll;  // Keep scrolling if already at the bottom.
	};
}

