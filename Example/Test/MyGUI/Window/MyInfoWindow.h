#pragma once

#include "../MyGUIContext.h"

namespace tezcat::Editor
{
	class MyInfoWindow : public GUIWindow
	{
	public:
		MyInfoWindow();
		~MyInfoWindow();

		void init() override;

	protected:
		void onRender() override;

	private:
		std::vector<float> mFrameTimeBuffer;
	};


}
