#pragma once

#include "include/Tiny.h"

namespace tezcat::Editor
{
	class EditorInputer : public InputerT<EditorInputer>
	{
	public:
		EditorInputer();
		virtual ~EditorInputer();

	public:
		void processInput(InputSystem* system) override;

		void onEnter() override;
		void onExit() override;
		void onPause() override;

		void mouseButtonCallBack(int button, int action, int mods) override;
		void mouseCursorPosCallBack(double xpos, double ypos) override;
		void mouseScrollPosCallBack(double xoffset, double yoffset) override;

	private:
		bool mLockMouse;
		float mLastX;
		float mLastY;
	};
}
