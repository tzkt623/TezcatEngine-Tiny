#pragma once

#include "../EditorContext.h"

namespace tezcat::Editor
{
	class EditorInfoWindow : public GUIWindow
	{
	public:
		EditorInfoWindow();
		~EditorInfoWindow();

		void init() override;

	protected:
		void onRender() override;

	private:
		std::vector<float> mFrameTimeBuffer;
	};


}
