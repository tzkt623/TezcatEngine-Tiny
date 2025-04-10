#pragma once


#include "../EditorContext.h"
namespace tezcat::Editor
{
	class EditorPipelineWindow : public GUIWindow
	{
		TINY_EDITOR_WINDOW_INSTANCE_H(EditorPipelineWindow);

	public:
		void open(GUI* gui) override;
		void close() override;

	protected:
		void onRender() override;
		void onNotRender() override;

	private:

	};
}
