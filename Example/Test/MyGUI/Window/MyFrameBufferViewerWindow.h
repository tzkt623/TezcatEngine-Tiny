#pragma once

#include "../MyGUIContext.h"
namespace tezcat::Editor
{
	class MyFrameBufferViewerWindow : public GUIWindow
	{
		TINY_EDITOR_WINDOW_INSTANCE_H(MyFrameBufferViewerWindow)
	public:
		void init() override;
		void onRender() override;
	};
}
