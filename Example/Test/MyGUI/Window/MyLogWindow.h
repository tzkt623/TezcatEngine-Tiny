#pragma once

#include "../MyGUIContext.h"

class MyLogWindow : public GUIWindow
{
public:
	MyLogWindow();
	virtual ~MyLogWindow();
private:

protected:
	void onRender() override;
	void onUpdate() override;

	void clear();
	void addLog(const char* fmt, ...) IM_FMTARGS(2);
private:
	ImGuiTextBuffer mTextBuffer;
	ImGuiTextFilter mFilter;
	ImVector<int> mLineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
	bool mAutoScroll;  // Keep scrolling if already at the bottom.
};


