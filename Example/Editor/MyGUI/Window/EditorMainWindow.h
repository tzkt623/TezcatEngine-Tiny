#pragma once

#include "../EditorContext.h"

namespace tezcat::Editor
{
	class EditorMainWindow : public GUIWindow
	{
	public:
		EditorMainWindow();
		virtual ~EditorMainWindow();

		void init() override;

	protected:
		void onRender() override;

	public:
		bool begin() override;
		void end() override;

	private:
		ImGuiWindowFlags mWindowFlags;
		ImGuiDockNodeFlags mDockspaceFlags;
	};
}

