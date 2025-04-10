#pragma once
#include "include/Tiny.h"

namespace tezcat::Editor
{
	class EditorEvent
	{
		EditorEvent() = delete;
		~EditorEvent() = delete;

	public:
		static Event* get() { return mEvent; }

	private:
		static Event* mEvent;
	};

	enum EditorEventID : EventID
	{
		Window_ObjectSelected = 0,
		Window_OpenShaderEditor,
		Window_OpenImageViwer,
		Window_OpenShaderManager,
		Count
	};
}
