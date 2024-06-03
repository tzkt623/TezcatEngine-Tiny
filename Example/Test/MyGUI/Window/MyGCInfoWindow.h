#pragma once

#include "../MyGUIContext.h"

namespace tezcat::Editor
{
	class MyGCInfoWindow : public GUIWindow
	{
		TINY_EDITOR_WINDOW_INSTANCE_H(MyGCInfoWindow)

	protected:
		void onRender() override;

	};
}

